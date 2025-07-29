#include "hardware.h"

// Variables globales
uint8_t controlValues[NUM_CONTROLS];
uint8_t previousValues[NUM_CONTROLS];

// Mapeo: índice de control → grupo funcional (fijo)
static const uint8_t grupoPorControl[NUM_CONTROLS] = {
  // Grupo 0 = SUB
  0,0,0,0,0,0,
  // Grupo 1 = OSC1
  1,1,1,1,1,1,
  // Grupo 2 = OSC2
  2,2,2,2,2,2,
  // Grupo 3 = MIX
  3,3,3,3,3,3,
  // Grupo 4 = ENV1
  4,4,4,4,4,4,
  // Grupo 5 = ENV2
  5,5,5,5,5,5,
  // Grupo 6 = LFO1
  6,6,6,6,6,6,
  // Grupo 7 = LFO2
  7,7,7,7,7,7,
  // Grupo 8 = FX
  8,8,8,8,8,
  // Grupo 9 = SWITCHES
  9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9
};

// Mapeo: grupo funcional → canal MIDI (editable)
static uint8_t canalPorGrupo[10]; // 0–9

static bool hasChanged = false;

static void setMuxChannel(uint8_t channel) {
  digitalWrite(MUX_S0, channel & 0x01);
  digitalWrite(MUX_S1, (channel >> 1) & 0x01);
  digitalWrite(MUX_S2, (channel >> 2) & 0x01);
  digitalWrite(MUX_S3, (channel >> 3) & 0x01);
}

void initHardware() {
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);

  pinMode(MUX_ANALOG_SIGNAL, INPUT);

  for (uint8_t i = 0; i < NUM_ANALOG_MUX; i++) {
    pinMode(analogMuxEnables[i], OUTPUT);
    digitalWrite(analogMuxEnables[i], HIGH);
  }

  for (uint16_t i = 0; i < NUM_CONTROLS; i++) {
    controlValues[i] = 0;
    previousValues[i] = 255;
  }

  // Asignación por defecto: cada grupo funcional tiene su canal MIDI
  canalPorGrupo[0] = 1;  // SUB
  canalPorGrupo[1] = 2;  // OSC1
  canalPorGrupo[2] = 3;  // OSC2
  canalPorGrupo[3] = 4;  // MIX
  canalPorGrupo[4] = 5;  // ENV1
  canalPorGrupo[5] = 6;  // ENV2
  canalPorGrupo[6] = 7;  // LFO1
  canalPorGrupo[7] = 8;  // LFO2
  canalPorGrupo[8] = 9;  // FX
  canalPorGrupo[9] = 10; // SWITCHES
}

void scanControls() {
  hasChanged = false;
  uint16_t index = 0;

  for (uint8_t mux = 0; mux < NUM_ANALOG_MUX; mux++) {
    digitalWrite(analogMuxEnables[mux], LOW);

    for (uint8_t ch = 0; ch < 16; ch++) {
      if (index >= NUM_CONTROLS) break;

      setMuxChannel(ch);
      delayMicroseconds(5);

      int raw = analogRead(MUX_ANALOG_SIGNAL);
      uint8_t value = (index < NUM_KNOBS)
                    ? map(raw, 0, 1023, 0, 127)
                    : (raw > 500 ? 127 : 0);

      controlValues[index] = value;

      if (abs((int)value - (int)previousValues[index]) >= 2) {
        previousValues[index] = value;
        hasChanged = true;
      }

      index++;
    }

    digitalWrite(analogMuxEnables[mux], HIGH);
  }
}

bool controlHasChanged() {
  return hasChanged;
}

uint8_t getControlValue(uint16_t index) {
  if (index >= NUM_CONTROLS) return 0;
  return controlValues[index];
}

bool isControlAnalog(uint16_t index) {
  return index < NUM_KNOBS;
}

uint8_t getControlChannel(uint16_t index) {
  if (index >= NUM_CONTROLS) return 1;
  uint8_t group = grupoPorControl[index];
  return canalPorGrupo[group];
}

void setGroupChannel(uint8_t groupId, uint8_t midiChannel) {
  if (groupId < 10 && midiChannel >= 1 && midiChannel <= 16) {
    canalPorGrupo[groupId] = midiChannel;
  }
}
void setControlChannel(uint16_t index, uint8_t channel) {
  // Esta función mantiene compatibilidad con #CHANNEL:<index>:<channel>
  // Anula el canal por grupo directamente
  if (index >= NUM_CONTROLS || channel < 1 || channel > 16) return;

  // Opcional: calcular el grupo afectado
  uint8_t grupo = grupoPorControl[index];
  canalPorGrupo[grupo] = channel;
}
