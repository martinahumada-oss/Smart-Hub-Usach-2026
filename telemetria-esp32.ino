/*
 * PROYECTO: Sistema de Telemetria de Energia
 * DEPARTAMENTO: Ingenieria Electrica (DIE)
 * ARCHIVO: telemetria_esp32.ino
 */

#include <WiFi.h>
#include <PubSubClient.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>

// Configuracion de hardware (Pines MAX485)
#define PIN_RECIBIR    16  // RX2
#define PIN_TRANSMITIR 17  // TX2
#define PIN_CONTROL    4   // DE / RE

// Parametros de red y servidores
const char* NOMBRE_RED    = "Wi-Fi_DIE_USACH";
const char* CLAVE_RED     = "ClaveSegura2026";
const char* SERVIDOR_MQTT = "192.168.1.50"; 
const char* TOPICO_ENVIO  = "die/laboratorios/macro";

WiFiClient clienteWifi;
PubSubClient clienteMqtt(clienteWifi);
ModbusMaster medidor;

// Control de flujo para el transceptor RS485
void modoHablar() {
  digitalWrite(PIN_CONTROL, HIGH);
}

void modoEscuchar() {
  digitalWrite(PIN_CONTROL, LOW);
}

// Gestion de conexiones de red y broker
void conectarRedes() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Conectando a Wi-Fi...");
    WiFi.begin(NOMBRE_RED, CLAVE_RED);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("\nWi-Fi Conectado.");
  }
  
  if (!clienteMqtt.connected()) {
    Serial.print("Conectando a broker MQTT...");
    clienteMqtt.setServer(SERVIDOR_MQTT, 1883);
    
    if (clienteMqtt.connect("ESP32_Central_Laboratorios")) {
      Serial.println("\nBroker MQTT Conectado.");
    } else {
      Serial.print("\nError de conexion MQTT, codigo: ");
      Serial.println(clienteMqtt.state());
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  pinMode(PIN_CONTROL, OUTPUT);
  modoEscuchar();
  
  // Inisializacion de puerto serial para Modbus a 9600 baudios
  Serial2.begin(9600, SERIAL_8N1, PIN_RECIBIR, PIN_TRANSMITIR);
  
  // Configurasion de nodo Modbus con ID 1
  medidor.begin(1, Serial2);
  medidor.preTransmission(modoHablar);
  medidor.postTransmission(modoEscuchar);
}

void loop() {
  conectarRedes();
  clienteMqtt.loop();
  
  uint8_t resultado;
  
  // Lectura de registros de variables instantaneas
  resultado = medidor.readInputRegisters(0x0000, 20);
  
  if (resultado == medidor.ku8MBSuccess) {
    StaticJsonDocument<300> cajaDatos;
    
    // Asignasion y escalado de variables electricas
    cajaDatos["voltaje"]    = medidor.getResponseBuffer(0x0000) / 10.0;
    cajaDatos["corriente"]  = medidor.getResponseBuffer(0x0008) / 1000.0;
    cajaDatos["potencia"]   = medidor.getResponseBuffer(0x0012) / 1000.0;
    cajaDatos["eficiencia"] = medidor.getResponseBuffer(0x001E) / 100.0;
    
    // Lectura del registro de energia acumulada
    medidor.readInputRegisters(0x0100, 2);
    float energiaTotal      = medidor.getResponseBuffer(0x0000) / 10.0;
    cajaDatos["energia_total"] = energiaTotal;
    
    // Indicadores de sotenibilidad e impacto economico
    cajaDatos["co2_mitigado"] = energiaTotal * 0.38;   // Factor de emision chileno
    cajaDatos["costo_pesos"]  = energiaTotal * 140.0;  // Tarifa estimada CLP/kWh
    
    // Serialisacion y envio de carga util JSON
    char textoListo[300];
    serializeJson(cajaDatos, textoListo);
    
    clienteMqtt.publish(TOPICO_ENVIO, textoListo);
    Serial.print("Payload enviado: ");
    Serial.println(textoListo);
    
  } else {
    Serial.print("Fallo de lectura Modbus. Codigo de error: ");
    Serial.println(resultado, HEX);
  }
  
  delay(5000);
}
