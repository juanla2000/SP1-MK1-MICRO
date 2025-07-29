#include <MIDIUSB.h>
#include "midi.h"
#include "hardware.h"
#include "routing.h"
#include "control_map.h"
#include "config.h"  // ← aquí se definen UART_RX y UART_TX
#include <avr/pgmspace.h>
#include <SoftwareSerial.h>

// Declaraciones adelantadas
void parseUartCommand(const String& command);

// Variable global del .ino
extern bool muteOutput;

// Puerto UART hacia Mk2
SoftwareSerial mk2Serial(UART_RX, UART_TX);

void initMIDI() {
  mk2Serial.begin(31250); // velocidad MIDI estándar
}

void updateMIDI() {
  static String uartBuffer = "";

  while (mk2Serial.available()) {
    char c = mk2Serial.read();
    if (c == '\n') {
      parseUartCommand(uartBuffer);
      uartBuffer = "";
    } else {
      uartBuffer += c;
    }
  }
}

void parseUartCommand(const String& command) {
  if (command.startsWith("#SET:")) {
    int sep1 = command.indexOf(':', 5);
    int sep2 = command.indexOf(':', sep1 + 1);
    if (sep1 > 0 && sep2 > sep1) {
      uint16_t idx = command.substring(5, sep1).toInt();
      uint8_t val = command.substring(sep1 + 1, sep2).toInt();
      uint8_t chn = command.substring(sep2 + 1).toInt();

      if (idx < NUM_CONTROLS) {
        previousValues[idx] = 255;
        controlValues[idx] = val;

        ControlDefinition temp;
        memcpy_P(&temp, &controlMap[idx], sizeof(ControlDefinition));
        sendCC(temp.cc, val, chn);
      }
    }
  } else if (command.startsWith("#CHANNEL:")) {
    int sep = command.indexOf(':', 9);
    if (sep > 0) {
      uint16_t idx = command.substring(9, sep).toInt();
      uint8_t newChannel = command.substring(sep + 1).toInt();
      if (idx < NUM_CONTROLS) {
        // No se puede reasignar canal en PROGMEM, se ignora
      }
    }
  } else if (command.startsWith("#MUTE")) {
    muteOutput = true;
  } else if (command.startsWith("#UNMUTE")) {
    muteOutput = false;
  }
}

void sendToMk2(const String& data) {
  mk2Serial.println(data);
}

// Envía un mensaje CC estándar por USB MIDI
void sendCC(uint8_t cc, uint8_t value, uint8_t channel) {
  midiEventPacket_t event = {0x0B, static_cast<uint8_t>(0xB0 | (channel & 0x0F)), cc, value};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();

  #if DEBUG
    DBG_PRINT("[sendCC] CC: ");
    DBG_PRINT(cc);
    DBG_PRINT(" Val: ");
    DBG_PRINT(value);
    DBG_PRINT(" Ch: ");
    DBG_PRINTLN(channel);
  #endif
}