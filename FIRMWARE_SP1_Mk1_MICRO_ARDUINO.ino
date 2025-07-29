// SP1_Mk1.ino
extern bool muteOutput;
// Núcleo del firmware para el módulo SP1 Mk1 (Arduino Micro)

#include "config.h"
#include "hardware.h"
#include "midi.h"
#include "routing.h"
#include "control_map.h"
#include <MIDIUSB.h>
#include <avr/pgmspace.h>


bool muteOutput = false; // control global para #MUTE/#UNMUTE

void checkIncomingUSBMIDI() {
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      uint8_t type = rx.byte1 & 0xF0;
      uint8_t d1 = rx.byte2;
      uint8_t d2 = rx.byte3;

      #if DEBUG
        Serial.print("[USB MIDI IN] Type: 0x");
        Serial.print(type, HEX);
        Serial.print(" D1: ");
        Serial.print(d1);
        Serial.print(" D2: ");
        Serial.println(d2);
      #endif

      processMIDIMessage(rx.byte1, rx.byte2, rx.byte3);
    }
  } while (rx.header != 0);
}

void setup() {
  #if DEBUG
    Serial.begin(115200);
    while (!Serial); // Espera conexión si es por USB
    DBG_PRINTLN("[SP1_Mk1] Iniciando...");
  #endif

  initHardware();
  initMIDI();
  initRouting();

  #if DEBUG
    DBG_PRINTLN("[SP1_Mk1] Setup completo");
  #endif

  if (!muteOutput) {
    midiEventPacket_t noteOn = {0x09, 0x90, 60, 100};
    MidiUSB.sendMIDI(noteOn);
    MidiUSB.flush();
    delay(100);
    midiEventPacket_t noteOff = {0x08, 0x80, 60, 0};
    MidiUSB.sendMIDI(noteOff);
    MidiUSB.flush();
  }
}

void loop() {
  scanControls();         // Escanea todos los potenciómetros e interruptores
  updateMIDI();           // Procesa comandos MIDI tipo #SET, #CHANNEL, etc.
  checkMk2Connection();   // Reenvía ping y evalúa si Mk2 responde
  checkIncomingUSBMIDI(); // Escanea MIDI entrante por USB

  if (controlHasChanged() && !muteOutput) {
    for (uint16_t i = 0; i < NUM_CONTROLS; i++) {
      if (previousValues[i] != controlValues[i]) {
        previousValues[i] = controlValues[i];

        ControlDefinition temp;
        memcpy_P(&temp, &controlMap[i], sizeof(ControlDefinition));

        uint8_t cc = temp.cc;
        uint8_t value = controlValues[i];
        uint8_t channel = temp.canal_midi;

        if (isStandaloneMode()) {
          value = constrain(value, 10, 120);
        }

        sendCC(cc, value, channel);
      }
    }
  }
}
