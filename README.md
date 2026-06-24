# Especificaciones de la funcionalidad
La logica es capturar las variables electricas del tablero general del DIE, procesarlas con un microcontrolador y transmitirlas de forma segura a una base de datos local para exponer el dashboard en la entrada del departamento
## Hardware 
* **Microcontrolador:** ESP32 NodeMCU
* **Mediciones:** Modbus-RTU sobre RS485 (ej. SDM630 o DTSU666). Esto calcula tensión, corriente, potencia activa, potencia reactiva y Factor de Potencia ($\cos \phi$) en tiempo real mediante lectura síncrona de formas de onda.
* **Interfaz de datos:** Transceptor TTL a RS485 (MAX485) para conectar el ESP32 al bus industrial.
* **Alimentacion:** En lugar de usar un transformador de teléfono común enchufado a la pared (que se suelta o genera ruido), instalamos una Fuente de Poder Conmutada (Switching) para riel DIN
* **Metodo de Medicion:** El metodo de medicion no es invasivo se caracteriza por utilizar transformadores de corriente (TC) de nucleo partido que actuan como pinzas magneticas ue se enlazan de forma directa los conductores de cada fase, sin alterar tableros del departamento. Para la medicion de tension se toman mediante derivaciones protegidas por fusibles
## Interfaz de Datos y Topología de Bus
Para eliminar ruidos electromagnéticos y cubrir grandes distancias, los medidores se conectan en serie mediante un **Bus RS485 (Modbus-RTU)**. Un único **ESP32 central** lee secuencialmente a todos los medidores del pasillo usando un par de cables trenzados, reduciendo drásticamente los costos de hardware.

## Variables de Telemetría (Carga útil JSON)
El ESP32 emite cada 5 segundos un paquete de datos en formato JSON hacia el broker MQTT local con las siguientes variables físicas e indicadores calculados:

### 1. Variables Eléctricas Instantáneas (Metrología Real)
* `v_rms`: Voltaje de línea promedio ($V$). Permite vigilar la estabilidad del suministro del departamento.
* `i_rms`: Corriente consolidada de las tres fases ($A$).
* `p_act`: Potencia Activa total ($kW$). Registra el consumo real de las máquinas y luminarias en funcionamiento.
* `p_react`: Potencia Reactiva total ($kVAr$). Monitorea la energía inductiva para evitar penalizaciones en la factura eléctrica.
* `f_pot`: Factor de Potencia ($\cos \phi$). Variable crítica en los laboratorios de potencia para medir el nivel de ineficiencia de los ensayos.

### 2. Variables Acumuladas e Indicadores de Impacto
* `e_act`: Energía Activa acumulada ($kWh$). Base matemática para las auditorías por bloque horario.
* `co2_mit`: Huella de Carbono mitigada ($kgCO_2eq$). Calculada automáticamente en el nodo usando el factor de emisión de la matriz eléctrica chilena ($\approx 0.38 \text{ kgCO}_2\text{/kWh}$).
* `cost_clp`: Costo económico acumulado ($CLP$). Proyección monetaria del consumo basada en la tarifa regulada vigente de la universidad.

---

