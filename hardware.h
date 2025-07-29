#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>
#include "config.h"

// Inicializa los pines y multiplexores
void initHardware();

// Escanea todos los controles físicos (analógicos)
void scanControls();

// Devuelve true si algún valor cambió desde la última lectura
bool controlHasChanged();

// Devuelve el valor actual de un control (0–127)
uint8_t getControlValue(uint16_t index);

// Determina si un control es potenciómetro o interruptor
bool isControlAnalog(uint16_t index);

// Devuelve el canal MIDI asignado a un control
uint8_t getControlChannel(uint16_t index);

// Reasigna el canal MIDI de un grupo funcional
void setGroupChannel(uint8_t groupId, uint8_t midiChannel);

// Reasigna el canal de un control concreto (útil para #CHANNEL)
void setControlChannel(uint16_t index, uint8_t channel);  // ← esta línea soluciona el error

// Variables globales accesibles desde otros módulos
extern uint8_t controlValues[NUM_CONTROLS];
extern uint8_t previousValues[NUM_CONTROLS];

#endif
