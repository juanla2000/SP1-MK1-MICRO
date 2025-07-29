#pragma once

#include <Arduino.h>

// -------- Configuración general --------
#define DEBUG 1
#define MODO_AUTONOMO_INICIAL true

// -------- Pines para los MUX --------
#define MUX_S0 2
#define MUX_S1 3
#define MUX_S2 4
#define MUX_S3 5
#define MUX_ANALOG_SIGNAL A0

// -------- Comunicación MIDI --------
#define MIDI_BAUDRATE_DIN 31250
#define MIDI_BAUDRATE_UART 115200

// -------- UART física con Mk2 --------
#define UART_RX 16  // Recepción desde Mk2
#define UART_TX 17  // Envío hacia Mk2

// -------- Depuración por USB --------
#if DEBUG
  #define DBG_PRINT(x)    Serial.print(x)
  #define DBG_PRINTLN(x)  Serial.println(x)
#else
  #define DBG_PRINT(x)
  #define DBG_PRINTLN(x)
#endif

// -------- Variables externas --------
extern bool muteOutput;
