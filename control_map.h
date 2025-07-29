#pragma once

#include <Arduino.h>

#define NUM_CONTROLS 108

struct ControlDefinition {
  const char* nombre;
  const char* grupo;
  uint8_t mux;
  uint8_t canal_mux;
  uint8_t cc;
  uint8_t canal_midi;
};

// Declaración del array en PROGMEM
extern const ControlDefinition controlMap[NUM_CONTROLS];
