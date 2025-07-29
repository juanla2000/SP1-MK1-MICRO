#include "routing.h"
#include "midi.h"
#include "config.h"

static bool mk2Alive = false;
static uint32_t lastMk2Ping = 0;
static uint32_t lastMk2Response = 0;

void initRouting() {
    mk2Alive = false;
    lastMk2Ping = millis();
    lastMk2Response = millis();
}

void checkMk2Connection() {
    uint32_t now = millis();

    // Enviar "ping" cada 500 ms
    if (now - lastMk2Ping > 500) {
        sendToMk2(0xF8, 0x00, 0x00);  // MIDI Clock como ping
        lastMk2Ping = now;
    }

    // Si no responde en 2 segundos, asumimos que está desconectado
    if (now - lastMk2Response > 2000) {
        mk2Alive = false;
    }
}

bool isMk2Connected() {
    return mk2Alive;
}

bool isMk3Connected() {
    return false; // Futuro
}

bool isStandaloneMode() {
    return !(isMk2Connected() || isMk3Connected());
}

void processMIDIMessage(uint8_t type, uint8_t data1, uint8_t data2) {
    // Detectar respuesta de Mk2
    if (type == 0xF8) {
        lastMk2Response = millis();
        mk2Alive = true;
    }

    // Por ahora reenviamos todos los CC a través de sendCC
    if ((type & 0xF0) == 0xB0) {
        sendCC(data1, data2, 1);
    }

    // Futuro: puedes enrutar Note On/Off u otros tipos
}
