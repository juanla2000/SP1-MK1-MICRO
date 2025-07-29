#include "midi.h"
#include "config.h"
#include "hardware.h"
#include "routing.h"

#include <MIDIUSB.h>
#include <SoftwareSerial.h>

// UART hacia SP1 Mk2 (DIN MIDI físico)
SoftwareSerial MIDI_UART(MK2_RX_PIN, MK2_TX_PIN);

// Buffer para comandos por UART desde Mk2
static String uartCommandBuffer = "";

void initMIDI() {
    MIDI_UART.begin(31250);
}

void parseUartCommand(const String& cmd) {
    if (cmd.startsWith("#SET:")) {
        int firstColon = cmd.indexOf(':');
        int secondColon = cmd.indexOf(':', firstColon + 1);
        if (secondColon > 0) {
            int cc = cmd.substring(firstColon + 1, secondColon).toInt();
            int val = cmd.substring(secondColon + 1).toInt();
            if (cc >= 0 && cc < NUM_CONTROLS && val >= 0 && val <= 127) {
                previousValues[cc] = 255;
                controlValues[cc] = val;
                sendCC(cc, val, getControlChannel(cc));
            }
        }
    }

    // Aquí podrías incluir otros comandos tipo #MUTE, #UNMUTE, etc.
    // No se llama a processMIDIMessage() porque cmd no es binario
}

void updateMIDI() {
    while (MIDI_UART.available()) {
        char c = MIDI_UART.read();
        if (c == '\n') {
            parseUartCommand(uartCommandBuffer);
            uartCommandBuffer = "";
        } else {
            uartCommandBuffer += c;
        }
    }
}

void sendCC(uint8_t cc, uint8_t val, uint8_t channel) {
    if (channel == 0 || channel > 16) return;

    uint8_t status = 0xB0 | ((channel - 1) & 0x0F);

    // MIDI UART (DIN MIDI)
    MIDI_UART.write(status);
    MIDI_UART.write(cc);
    MIDI_UART.write(val);

    // MIDI USB
    midiEventPacket_t event = {0x0B, status, cc, val};
    MidiUSB.sendMIDI(event);
    MidiUSB.flush();
}

void sendToMk2(uint8_t type, uint8_t data1, uint8_t data2) {
    MIDI_UART.write(type);
    MIDI_UART.write(data1);
    MIDI_UART.write(data2);
}
