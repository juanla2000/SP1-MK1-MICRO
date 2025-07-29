#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define NUM_KNOBS     91
#define NUM_SWITCHES  17
#define NUM_CONTROLS (NUM_KNOBS + NUM_SWITCHES)

#define DEBUG         1
#define MODO_AUTONOMO_INICIAL true

#define MUX_S0 2
#define MUX_S1 3
#define MUX_S2 4
#define MUX_S3 5

#define MUX_ANALOG_SIGNAL A0

#define NUM_ANALOG_MUX 9
const uint8_t analogMuxEnables[NUM_ANALOG_MUX] = {
    7, 8, 9, 10, 11, 12, 13, 14, 15
};

#define MIDI_BAUDRATE_DIN 31250
#define MIDI_BAUDRATE_UART 115200

#define MK2_RX_PIN 16
#define MK2_TX_PIN 17

#if DEBUG
  #define DBG_PRINT(x)    Serial.print(x)
  #define DBG_PRINTLN(x)  Serial.println(x)
#else
  #define DBG_PRINT(x)
  #define DBG_PRINTLN(x)
#endif

// ← ✅ AÑADIR ESTA LÍNEA AL FINAL:
extern bool muteOutput;

#endif
