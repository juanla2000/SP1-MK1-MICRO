#include "hardware.h"
#include "config.h"
#include "control_map.h"
#include <avr/pgmspace.h>

uint8_t controlValues[NUM_CONTROLS] = {0};
uint8_t previousValues[NUM_CONTROLS] = {255};

const uint8_t mux_input_pins[9] = {7, 8, 9, 10, 11, 12, 13, 14, 15};

#define PIN_S0 MUX_S0
#define PIN_S1 MUX_S1
#define PIN_S2 MUX_S2
#define PIN_S3 MUX_S3

static bool hasChanged = false;
static uint8_t canalPorGrupo[16];

static void setMuxChannel(uint8_t channel) {
  digitalWrite(PIN_S0, channel & 0x01);
  digitalWrite(PIN_S1, (channel >> 1) & 0x01);
  digitalWrite(PIN_S2, (channel >> 2) & 0x01);
  digitalWrite(PIN_S3, (channel >> 3) & 0x01);
}

void initHardware() {
  pinMode(PIN_S0, OUTPUT);
  pinMode(PIN_S1, OUTPUT);
  pinMode(PIN_S2, OUTPUT);
  pinMode(PIN_S3, OUTPUT);

  for (uint8_t i = 0; i < 9; i++) {
    pinMode(mux_input_pins[i], INPUT);
  }

  for (uint16_t i = 0; i < NUM_CONTROLS; i++) {
    controlValues[i] = 0;
    previousValues[i] = 255;
  }

  for (uint8_t i = 0; i < sizeof(canalPorGrupo); i++) {
    canalPorGrupo[i] = 1 + i;
  }
}

void scanControls() {
  hasChanged = false;

  for (uint16_t i = 0; i < NUM_CONTROLS; i++) {
    ControlDefinition ctrl;
    memcpy_P(&ctrl, &controlMap[i], sizeof(ControlDefinition));

    setMuxChannel(ctrl.canal_mux);
    delayMicroseconds(3);
    uint16_t rawValue = analogRead(mux_input_pins[ctrl.mux]);
    uint8_t mappedValue = rawValue >> 3;

    if (controlValues[i] != mappedValue) {
      previousValues[i] = controlValues[i];
      controlValues[i] = mappedValue;
      hasChanged = true;
    }
  }
}

bool controlHasChanged() {
  return hasChanged;
}

uint8_t getControlValue(uint16_t index) {
  return controlValues[index];
}

bool isControlAnalog(uint16_t index) {
  return true;
}

uint8_t getControlChannel(uint16_t index) {
  ControlDefinition ctrl;
  memcpy_P(&ctrl, &controlMap[index], sizeof(ControlDefinition));
  return ctrl.canal_midi;
}

void setControlChannel(uint16_t index, uint8_t channel) {
  // No se puede modificar directamente en PROGMEM
}

void setGroupChannel(const char* grupo, uint8_t midiChannel) {
  // No se puede modificar directamente en PROGMEM
}
