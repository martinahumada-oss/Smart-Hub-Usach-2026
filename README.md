# Smart-Hub-Usach-2026
Repositorio que organiza el desarrollo de la propuesta del monitoreo de la eficiencia energetica en el die

## Aspecto General
  **Nombre del proyecto:**  Plataforma IoT de monitoreo activo de la eficiencia energetica del DIE
  **Lineas Tematicas:**  Sostenibilidad Energética, Eficiencia Operativa e Infraestructura           Conectada.
  **Objetivo:** Hacer del departamento de Ingenieria Electrica en el edificio mas eficiente de la   USACH
## Modelamiento del sistema
El sistema se dividira en tres etapas secuenciales que abarcan desde la captura fisica de datos a la interfaz del usuario y la toma de decisiones automatizada
### Etapa 1: DashBoard General
<img width="1142" height="642" alt="image" src="https://github.com/user-attachments/assets/34387d58-b165-426c-86db-9b5961d6a61a" />

* **Objetivo:** Instalacion de una herramienta visual en la entrada del die que exponga el consumo energetico general del departamento
* **Variables a exponer:**
* Consumo instantáneo ($kW$) y demanda acumulada ($kWh$) de todo el departamento.
* Equivalencia en Huella de Carbono mitigada ($kgCO_2eq$).
* Costo proyectado de la facturación eléctrica mensual ($CLP$)
* **Herramientas:** Programar mediante Python/Node-RED -> Base de datos (InfluxDB) -> Visualización de datos **Grafana**.
### Etapa 2: DashBoard especificas para sesiones de laboratorio
* **Entrega:** Bot Telegram
* **Objetivo:** Enviar informes especificos sobre el consumo energetico de cada sesion de laboratorio
* **Variables a exponer:**
* **Energía Activa ($kWh$):** Consumo neto del bloque académico.
* **Demanda Máxima ($kW$):** Peak de potencia alcanzado durante la sesión.
* **Factor de Potencia ($\cos \phi$):** Monitoreo crítico para identificar ineficiencias por consumo de energía reactiva (penalizaciones tarifarias), vital en laboratorios con cargas inductivas masivas (ej: Lab de Máquinas Eléctricas).
* **Costo Operacional ($CLP$):** Traducción del consumo a dinero real según tarifa regulada USACH.
### Etapa 3: Automatizacion de la eficiencia
* **Objetivo: ** Controlar el consumo energetico mediante apagados automaticos y escaneo de usos
* **Logica de Control: **
* * **Seguridad Operacional:** El sistema **NUNCA ejecuta un corte abrupto de energía de forma autónoma** si detecta un consumo significativo, solo detecta equipos en stand by como prevencion (<50 W)
  * **Fase de Alerta:** Al finalizar un bloque horario, si el consumo está sobre el umbral base, se notifica automáticamente a los encargados vía Telegram.
  * **Fase de Actuación Segura:** A las 22:00 hrs, el corte automático de enchufes no críticos solo se ejecutará si el consumo es menor a 50W (stand-by) o si un administrador lo autoriza manualmente vía Telegram
