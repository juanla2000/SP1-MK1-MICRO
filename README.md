# SP1-MK1 Arduino Micro
Sp1 Mk1 Tangible MIDI Control

# README SP1\_ARDUINO\_MICRO

Este archivo describe el estado actual y las tareas pendientes para el firmware MIDI implementado sobre la placa Arduino Micro, que forma parte del proyecto FIRMWARE\_MIDI\_SP1 en combinación con un ESP32-S3.

## 📌 Estado Actual del Firmware

### Funciones Actuales Implementadas

* **Lectura de controles físicos:**

  * Se realiza la lectura directa de potenciómetros y botones conectados analógicamente y digitalmente.

* **Comunicación Serial con ESP32:**

  * Recepción básica de mensajes por UART desde el ESP32 para actualizar valores MIDI internos y reenviarlos al PC.

* **Envío de Mensajes MIDI:**

  * Los controles físicos envían mensajes MIDI CC directamente vía USB usando la librería MIDIUSB estándar.

* **Sincronización con ESP32:**

  * Recibe por UART mensajes CC provenientes del ESP32 para reenviarlos por USB al DAW o sintetizador.

## 📂 Estructura Actual del Código

Actualmente, el código está íntegramente contenido en un único archivo `.ino`:

* **SP1\_ARDUINO\_MICRO.ino**: Contiene toda la lógica actual para lectura analógica/digital, envío de MIDI USB y recepción UART.

---

## 🛠️ Tareas Pendientes para Implementar

A continuación se enumeran las mejoras pendientes identificadas en base a las prácticas recomendadas y al análisis comparativo realizado con otros proyectos como Cthulhinho:

### 🔴 Alta Prioridad:

* **Modularización del código**:

  * Separar en archivos específicos la lógica MIDI, comunicación UART y controles analógicos:

    * `comunicacion_micro.h/.cpp` (MIDI USB y comunicación UART segura)
    * `controles_micro.h/.cpp` (Lectura y filtrado de entradas analógicas/digitales)

* **Comunicación UART segura con ESP32**:

  * Implementar protocolo estructurado con delimitadores y checksum básico para asegurar integridad de datos.

* **Filtrado avanzado de entradas analógicas**:

  * Añadir filtros digitales (media móvil ponderada o mediana) para estabilizar lecturas analógicas.

### 🟠 Media Prioridad:

* **Optimización del envío de mensajes MIDI USB**:

  * Agregar bufferización y enviar mensajes solo cuando existan cambios significativos en los controles.

* **MIDI Thru**:

  * Implementar reenvío transparente de mensajes MIDI recibidos por UART hacia USB.

### 🟢 Baja Prioridad:

* **Gestión no bloqueante**:

  * Implementar rutinas periódicas basadas en `millis()` en vez de funciones bloqueantes (`delay()`).

* **Debug avanzado por puerto Serial**:

  * Añadir mensajes estructurados con niveles de importancia (INFO, ERROR, DEBUG).

* **Sistema basado en eventos para mensajes CC**:

  * Añadir un sistema eficiente que permita reaccionar rápidamente a cambios provenientes del ESP32.

---

## 🔗 Interacción con el Proyecto ESP32

El Arduino Micro funciona exclusivamente como controlador MIDI principal:

* **Recibe desde el ESP32 por UART:**

  * Valores CC para actualizar internamente los controles MIDI enviados por USB.

* **No maneja Rotary Encoder:**

  * El encoder rotativo para menús y presets se encuentra conectado directamente al ESP32.

* **Interfaz USB MIDI directa al PC:**

  * Envío de todos los mensajes MIDI generados desde los controles físicos y provenientes del ESP32.

---

## 🚀 Próximos Pasos Recomendados

Se recomienda iniciar con la modularización inmediata del código actual, seguida por la implementación de una comunicación UART segura y la mejora de la estabilidad de lectura de controles analógicos.

Este enfoque proporcionará una base sólida, escalable y fácilmente mantenible para futuras mejoras y optimizaciones del firmware.
