# Funcionamiento del codigo programada para ESP32
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
## Parte 2: Procesamiento y ordenamiento de los datos



