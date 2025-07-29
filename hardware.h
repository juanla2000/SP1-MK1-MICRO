#pragma once

#include <Arduino.h>
#include "control_map.h"

void initHardware();
void scanControls();
bool controlHasChanged();
uint8_t getControlValue(uint16_t index);
bool isControlAnalog(uint16_t index);
uint8_t getControlChannel(uint16_t index);
void setControlChannel(uint16_t index, uint8_t channel);
void setGroupChannel(const char* grupo, uint8_t midiChannel);

extern uint8_t controlValues[NUM_CONTROLS];
extern uint8_t previousValues[NUM_CONTROLS];
