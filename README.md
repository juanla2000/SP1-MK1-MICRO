# SP1 Mk1 — Firmware v1.1 (Arduino Micro)

Este módulo es la base física del sistema **SP1 Tangible**, controlando hasta **108 entradas analógicas** (91 knobs + 17 switches) y actuando como **puente MIDI autónomo** entre USB, DIN y UART. Puede operar de forma independiente o como esclavo del módulo Mk2 (ESP32-S3), enviando y recibiendo comandos por UART.

---

## ⚙️ Hardware físico

- Arduino Micro (ATmega32u4)
- 9 multiplexores CD74HC4067 (16 canales cada uno)
- Pines S0–S3 compartidos entre los 9 MUX (D2–D5)
- Línea analógica común a A0
- Pines EN dedicados por MUX: D7 a D15
- UART entre Mk1 ↔ Mk2 por SoftwareSerial (pines 8 y 9)

---

## 🎛 Control de entradas

- `scanControls()` lee todos los controles como analógicos (zona muerta ≥2)
- Se detectan cambios y solo se envían valores modificados
- Cada control tiene un `grupo`, un `nombre`, un `CC`, un `canal MIDI`, y se define en `control_map.h`

---

## 🎹 Salidas y ruteo MIDI

- **USB MIDI** (nativo, vía `MIDIUSB.h`)
- **DIN MIDI** (`Serial1` — UART real)
- **UART MIDI** (a Mk2 por SoftwareSerial)

El sistema reenvía CC y notas entre interfaces:
- USB ↔ DIN
- DIN ↔ UART
- USB ↔ UART

---

## 🔀 Enrutamiento MIDI dinámico

- Todos los mensajes entrantes USB se procesan con `checkIncomingUSBMIDI()` y se reenvían con `processMIDIMessage()`
- Comandos UART del Mk2 permiten modificar valores o canales sin tocar el hardware

---

## 🧪 Comandos UART (Mk2 → Mk1)

| Comando                | Acción                                                                 |
|------------------------|------------------------------------------------------------------------|
| `#SET:<idx>:<val>:<ch>`| Asigna valor `<val>` al control `<idx>` y lo envía como CC por canal `<ch>` |
| `#CHANNEL:<idx>:<ch>`  | Reasigna canal MIDI de un control específico                          |
| `#GROUPCH:<grupo>:<ch>`| Asigna canal MIDI a un grupo funcional completo                        |
| `#MUTE` / `#UNMUTE`    | Silencia o reactiva toda la salida MIDI                                |
| `#ID?`                 | Devuelve el identificador `SP1_MK1_V1.1`                               |

---

## 🧩 Grupos funcionales

Cada control pertenece a un grupo lógico:

| ID  | Grupo      | Canal MIDI por defecto |
|-----|------------|------------------------|
| 0   | SUB        | 1                      |
| 1   | OSC1       | 2                      |
| 2   | OSC2       | 3                      |
| 3   | MIX        | 4                      |
| 4   | ENV1       | 5                      |
| 5   | ENV2       | 6                      |
| 6   | LFO1       | 7                      |
| 7   | LFO2       | 8                      |
| 8   | FX         | 9                      |
| 9   | SWITCHES   | 10                     |

> Estos canales pueden modificarse dinámicamente desde el Mk2.

---

## 🧠 Funciones clave del firmware

- `initHardware()` – Configura pines, multiplexores y arrays de control.
- `initMIDI()` – Inicia MIDI USB y UART SoftwareSerial.
- `scanControls()` – Detecta cambios en los controles.
- `sendCC()` – Envía mensajes CC por USB y DIN.
- `updateMIDI()` – Procesa comandos recibidos por UART desde el Mk2.
- `checkIncomingUSBMIDI()` – Procesa mensajes entrantes por USB.
- `processMIDIMessage()` – Aplica lógica de reenvío y muting.
- `sendToMk2()` – Envía comandos MIDI codificados al Mk2.

---

## 📁 Estructura del código

```text
FIRMWARE_SP1_Mk1_MICRO_ARDUINO/
├── FIRMWARE_SP1_Mk1_MICRO_ARDUINO.ino   → Loop principal
├── config.h                             → Pines y flags globales
├── control_map.h / control_map.cpp      → Tabla de definición de controles (PROGMEM)
├── hardware.h / hardware.cpp            → Lectura física y agrupación de controles
├── midi.h / midi.cpp                    → Manejo de MIDI USB, DIN, UART
├── routing.h / routing.cpp              → Estado de conexión con Mk2
├── mux_input_pins.h / mux_input_pins.cpp→ Pines EN y configuración de multiplexores
