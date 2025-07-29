# SP1 Mk1 — Firmware v1.0 (Arduino Micro)

Este módulo forma parte del sistema **SP1 Tangible** y representa el **núcleo físico** del controlador: captura 108 controles (knobs y switches), los convierte a mensajes MIDI y se comunica con los módulos Mk2 (ESP32-S3) y Mk3 (Teensy).

---

## 🧠 Funciones clave

### 🔧 Hardware físico
- 91 potenciómetros + 17 switches leídos vía 9 multiplexores HC4067.
- Pines de selección de canal compartidos (S0–S3), señal común en A0.
- Se leen todos como **entradas analógicas** con zona muerta (`delta ≥ 2`) para evitar ruido.

### 🎛 Envío de datos
- Todos los CCs se envían por:
  - USB MIDI
  - DIN MIDI (`Serial1`)
  - UART a Mk2 (via `SoftwareSerial`)
- Envío eficiente: solo controles que cambian.
- Canal MIDI dinámico por grupo funcional (ver más abajo).

### 🔁 Puente MIDI completo
- El Mk1 actúa como **concentrador MIDI activo**, reenviando CCs tanto por USB como DIN.
- Procesa comandos UART tipo `#SET:` y los transforma en eventos MIDI.
- Además, el sistema es capaz de interpretar comandos desde USB y UART, y aplicar cambios directos al estado interno (muteo, reasignación, etc.).
- La función `updateMIDI()` también puede alimentar el ruteo dinámico entre módulos, integrándose con `processMIDIMessage()` si es necesario.
- El Mk1 enruta y reenvía todo lo que recibe (DIN ↔ USB ↔ UART), funcionando como **concentrador MIDI** entre módulos y externos.

---

## 📡 Comunicación UART (con Mk2)

### Comandos UART soportados:

| Comando                | Descripción                                                             |
|------------------------|-------------------------------------------------------------------------|
| `#SET:<cc>:<val>`      | Fuerza el valor del control `<cc>` a `<val>` y lo envía por MIDI        |
| `#GET_ALL`             | Envía por UART el valor de todos los controles                          |
| `#RESET_ALL`           | Reenvía todos los controles por MIDI, aunque no hayan cambiado          |
| `#MUTE` / `#UNMUTE`    | Silencia o reactiva la salida MIDI del Mk1                              |
| `#ID?`                 | Devuelve: `SP1_MK1_V1.0`                                                |
| `#CHANNEL:<cc>:<ch>`   | Reasigna el canal MIDI del control `<cc>` a `<ch>`                      |
| `#GROUPCH:<name>:<ch>` | Reasigna el canal MIDI del grupo funcional `<name>` a `<ch>`            |

---

## 🧩 Arquitectura por grupos

Cada control pertenece a un grupo funcional fijo, mapeado internamente en el Mk1:

| Grupo ID | Nombre     | Canal MIDI por defecto |
|----------|------------|------------------------|
| 0        | SUB        | 1                      |
| 1        | OSC1       | 2                      |
| 2        | OSC2       | 3                      |
| 3        | MIX        | 4                      |
| 4        | ENV1       | 5                      |
| 5        | ENV2       | 6                      |
| 6        | LFO1       | 7                      |
| 7        | LFO2       | 8                      |
| 8        | FX         | 9                      |
| 9        | SWITCHES   | 10                     |

> El canal de cada grupo puede ser modificado dinámicamente desde el Mk2 vía `#GROUPCH`.

---

## 📂 Estructura del código

```text
FIRMWARE_SP1_Mk1_MICRO_ARDUINO/
├── Sp1_Mk1.ino              → Loop principal y control general
├── config.h                 → Pines, constantes globales
├── hardware.cpp/h           → Lectura de multiplexores, agrupación, canal MIDI
├── midi.cpp/h               → MIDI USB/DIN/UART + comandos UART
├── routing.cpp/h            → Detección de Mk2 activo por UART
