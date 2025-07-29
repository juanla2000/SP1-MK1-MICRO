#pragma once

#include <Arduino.h>

// Inicializa el sistema de enrutamiento MIDI
void initRouting();

// Analiza y enruta todos los mensajes MIDI entrantes
void processMIDIMessage(uint8_t type, uint8_t data1, uint8_t data2);

// Verifica conexión con Mk2 (ping/respuesta por UART)
void checkMk2Connection();

// Detecta si Mk2 está presente
bool isMk2Connected();

// Detecta si Mk3 está presente (futuro)
bool isMk3Connected();

// Modo autónomo: true si Mk1 funciona sin Mk2/Mk3
bool isStandaloneMode();
