# Funcionamiento del codigo para ESP32
En este apartado se explica como se programara el microcontrolador **ESP32** para que converse con los medidores y envíe los datos a nuestra pantalla.
El codigo se desglosa en tres partes principales

## Parte 1 : Conversación Basica
Preparamos la conversación entre el medidor industrial usando un chip intermedio **MAX485** y especificamos las conexiones en los pines ademas de como controlar el tráfico de datos (si escucha o habla en la conversacion)
```cpp
#include <ModbusMaster.h> //Libreria que traduce el idioma del medidor industrial
//En el siguiente bloque se definen pines a los que ingresam los datos
#define PIN_RECIBIR 16//Escucha el medidor
#define PIN_TRANSMITIR 17 //Habla con medidor
#define PIN_CONTROL 4 // Decida si hablamos o escuchamos

ModbusMaster Medidor //Modelados el medidor en el codigo

// Creamos dos funciones simples para hablar o escuchar
void modoHablar() {
  digitalWrite(PIN_CONTROL, HIGH)
}
void modoEscuchar(){
  digitalWrite(PIN_CONTROÑ, LOW)
}

```
## Parte 2: Procesamiento de los datos
El microcontrolador recibe un montón de datos sin acomodar , esta parte del codigo toma los numeros y los guarda etiquetados en un **JSON**, de forma que cualquiera que lea los datos sabrá para que sirve cada valor.
```cpp
#include <ArduinoJson.h> //libreria que empaqueta los datos ordenados

void procesarYEnviarDatos(){
  //1. Se crea el contenedor .json que guarda los datos de la caja
  StaticJsonDocument<300> cajaDatos;
  //2. Tomamos la informacion entregada por el medidor y se le asigna su etiqueta         correspondiente
  cajaDatos["voltaje"] = medidor.getResponseBuffer(0x0000);
  cajaDatos["corriente"] = medidor.getResponseBuffer(0x0008);
  cajaDatos["potencia"] = medidor.getResponseBuffer(0x0012);
  cajaDatos["eficiencia"] = medidor.getResponseBuffer(0x001E);

  //3. Obtener los calculos de forma atomatica
  float enegiaTotal = medidor.getResponseBuffer(0x0100) / 10 //Calculo de KiloWatts   por hora acumulador;
  cajaDatos["energia_total"] = energiaTotal;
  cajaDatos["co2"] = energiaTotal*0.38; //Calculo de huella de carbono
  cajaDatos["costo_pesos"] = energiaTotal * 140.0; //Calculo de costo en pesos
  //4. Convertir toda la caja en un texto simple que pueda viajar por red
  char textoListo[300]
  serializeJson(cajaDatos, textoListo); 
}
```
## Parte 3: Envio de datos al servidor
Luego de ordenar los datos en la caja Json, se necesia que el ESP32 se conecte a la red y despache la informacion donde sea necesario.
Se utiliza un protocolo **MQTT** como mensajeria de los datos

```cpp
#include <WiFi.h> //libreria que permite la conexion del microcontrolador a WiFi
#include <PubSubClient.h> //libreria que permite el envio de los datos mediante MQTT

// 1. Datos de Red
const char* NOMBRE_RED = "" ;
const char* CLAVE_RED = "" ;

// 2. Direccion de servidor central
const char* SERVIDOR_MQTT = "";

WiFiClient clienteWifi;
PubSubClient clienteMqtt(clienteWifi);

void conectarRed(){
  // Busca la conexion si no esta conectado
  if (WiFi.status() != WL_CONNECTED){
    Wifi.begin(NOMBRE_RED, CLAVE_RED);
    while (Wifi.status() != WL_CONNECTED { delay(500); }
  }
  // Busca la conexion al servidor de datos
  if (!clienteMqtt.connected()){
  clienteMqtt.setServer(SERVIDOR_MQTT, 1883);
  clienteMqtt.connect("ESP32_Central_Laboratorios");  
  }
}

void despacharDatos(char* textoParaEnviar) {
  conectarRed() //se asegura que la conexion de red este activa

  //envio del texto al canal de la base de datos
  clienteMqtt.publish("die/laboratorios/macro", textoParaEnviar);
}
```
