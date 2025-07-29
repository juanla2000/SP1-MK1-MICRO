#ifndef MIDI_MANAGER_H
#define MIDI_MANAGER_H

#include <Arduino.h>

// Inicialización de puertos MIDI
void initMIDI();

// Procesa mensajes entrantes (USB, DIN, UART + comandos tipo #)
void updateMIDI();

// Enviar un CC por todos los destinos activos
void sendCC(uint8_t cc, uint8_t value, uint8_t channel);

// Enviar una nota
void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel);
void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel);

// Enviar datos binarios a Mk2 por UART
void sendToMk2(uint8_t type, uint8_t data1, uint8_t data2);


#endif
