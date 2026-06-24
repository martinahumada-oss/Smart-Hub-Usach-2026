# Especificaciones de la funcionalidad
La logica es capturar las variables electricas del tablero general del DIE, procesarlas con un microcontrolador y transmitirlas de forma segura a una base de datos local para exponer el dashboard en la entrada del departamento
## Hardware 
* **Microcontrolador:** ESP32 NodeMCU
* **Mediciones:** Modbus-RTU sobre RS485 (ej. SDM630 o DTSU666). Esto calcula tensión, corriente, potencia activa, potencia reactiva y Factor de Potencia ($\cos \phi$) en tiempo real mediante lectura síncrona de formas de onda.
* **Interfaz de datos:** Transceptor TTL a RS485 (MAX485) para conectar el ESP32 al bus industrial.
* **Alimentacion:** En lugar de usar un transformador de teléfono común enchufado a la pared (que se suelta o genera ruido), instalamos una Fuente de Poder Conmutada (Switching) para riel DIN
