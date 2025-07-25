#include <MIDIUSB.h>

// Comunicación UART con ESP32-S3
#define UART_TX_PIN 1  // TX del Arduino Micro (conectado al RX del ESP32-S3)
#define UART_RX_PIN 0  // RX del Arduino Micro (conectado al TX del ESP32-S3)
#define UART_BAUDRATE 31250  // Velocidad MIDI estándar

#define S0_PIN 2
#define S1_PIN 3
#define S2_PIN 4
#define S3_PIN 5

#define NUM_CONTROLES 108
#define NUM_MEDIAS 2  // Número reducido de muestras para filtro media móvil (ahorro RAM)

// Pines analógicos (MUX SIG) conectados a cada 74HC4051
const uint8_t muxPins[] = {A0, A1, A2, A3, A4, A5, 6, 7, 8};

struct ControlConfig {
  uint8_t mux;
  uint8_t pin;
  uint8_t cc;
  uint8_t canal;
};

// Tabla de configuración completa con nombres comentados por función
const ControlConfig controles[NUM_CONTROLES] PROGMEM = {
  // Sub Osc y Noise
  {0, 0, 16, 1},  // Sub Shape
  {0, 1, 17, 1},  // Sub Octave
  {0, 2, 18, 1},  // Sub Osc Pan
  {0, 3, 19, 1},  // Sub Osc Level
  {0, 4, 20, 1},  // Noise Phase
  {0, 5, 21, 1},  // Noise RandPhase
  {0, 6, 22, 1},  // Noise Pitch
  {0, 7, 23, 1},  // Noise Pan
  {0, 8, 24, 1},  // Noise Level

  // OSC A
  {1, 0, 26, 1},  // OSC A Octave
  {1, 1, 27, 1},  // OSC A Semi
  {1, 2, 28, 1},  // OSC A Fine
  {1, 3, 29, 1},  // OSC A Coarse Pitch
  {1, 4, 30, 1},  // OSC A Unison
  {1, 5, 31, 1},  // OSC A Unison Detune
  {1, 6, 32, 1},  // OSC A Unison Blend
  {1, 7, 33, 1},  // OSC A Phase
  {1, 8, 34, 1},  // OSC A Random Phase
  {1, 9, 35, 1},  // OSC A WT Position
  {1, 10, 36, 1}, // OSC A Warp
  {1, 11, 37, 1}, // OSC A Pan
  {1, 12, 38, 1}, // OSC A Volume

  // OSC B
  {2, 0, 42, 1},  // OSC B Octave
  {2, 1, 43, 1},  // OSC B Semi
  {2, 2, 44, 1},  // OSC B Fine
  {2, 3, 45, 1},  // OSC B Coarse Pitch
  {2, 4, 46, 1},  // OSC B Unison
  {2, 5, 47, 1},  // OSC B Unison Detune
  {2, 6, 48, 1},  // OSC B Unison Blend
  {2, 7, 49, 1},  // OSC B Phase
  {2, 8, 50, 1},  // OSC B Random Phase
  {2, 9, 51, 1},  // OSC B WT Position
  {2, 10, 52, 1}, // OSC B Warp
  {2, 11, 53, 1}, // OSC B Pan
  {2, 12, 54, 1}, // OSC B Volume

  // Master y filtro
  {3, 0, 58, 1},  // Master Volume
  {3, 1, 59, 1},  // Filter Cutoff
  {3, 2, 60, 1},  // Filter Resonance
  {3, 3, 61, 1},  // Filter Stereo
  {3, 4, 62, 1},  // Filter Drive
  {3, 5, 63, 1},  // Filter Variation
  {3, 6, 64, 1},  // Filter Mix
  {3, 7, 65, 1},  // Piano Portamento
  {3, 8, 66, 1},  // Piano Velocity Curve

  // Envolventes: Env1
  {4, 0, 74, 1},  // Env1 Attack
  {4, 1, 75, 1},  // Env1 Hold
  {4, 2, 76, 1},  // Env1 Decay
  {4, 3, 77, 1},  // Env1 Sustain
  {4, 4, 78, 1},  // Env1 Release
  // Env2
  {4, 5, 79, 1},  // Env2 Attack
  {4, 6, 80, 1},  // Env2 Hold
  {4, 7, 81, 1},  // Env2 Decay
  {4, 8, 82, 1},  // Env2 Sustain
  {4, 9, 83, 1},  // Env2 Release
  // Env3
  {4, 10, 84, 1}, // Env3 Attack
  {4, 11, 85, 1}, // Env3 Hold
  {4, 12, 86, 1}, // Env3 Decay
  {4, 13, 87, 1}, // Env3 Sustain
  {4, 14, 88, 1}, // Env3 Release

  // LFOs
  {5, 0, 90, 1},  // LFO1 Rate
  {5, 1, 91, 1},  // LFO1 Rise
  {5, 2, 92, 1},  // LFO1 Delay
  {5, 3, 93, 1},  // LFO1 Smooth
  {5, 4, 94, 1},  // LFO2 Rate
  {5, 5, 95, 1},  // LFO2 Rise
  {5, 6, 96, 1},  // LFO2 Delay
  {5, 7, 97, 1},  // LFO2 Smooth
  {5, 8, 98, 1},  // LFO3 Rate
  {5, 9, 99, 1},  // LFO3 Rise
  {5, 10, 100, 1},// LFO3 Delay
  {5, 11, 101, 1},// LFO3 Smooth

  // Macros
  {6, 0, 106, 1}, // Macro 1
  {6, 1, 107, 1}, // Macro 2
  {6, 2, 108, 1}, // Macro 3
  {6, 3, 109, 1}, // Macro 4
  {6, 4, 110, 1}, // Macro 5
  {6, 5, 111, 1}, // Macro 6
  {6, 6, 112, 1}, // Macro 7
  {6, 7, 113, 1}, // Macro 8
  {6, 8, 114, 1}, // Macro 9
  {6, 9, 115, 1}, // Macro 10
  {6, 10, 116, 1},// Macro 11
  {6, 11, 117, 1},// Macro 12
  {6, 12, 118, 1},// Macro 13
  {6, 13, 119, 1},// Macro 14
  {6, 14, 120, 1},// Macro 15
  {6, 15, 121, 1},// Macro 16

  // Switches: On/Off y asignaciones
  {7, 0, 136, 1}, // Sub On
  {7, 1, 137, 1}, // Noise On
  {7, 2, 138, 1}, // OSC A On
  {7, 3, 139, 1}, // OSC B On
  {7, 4, 140, 1}, // Filter On
  {7, 5, 141, 1}, // OSC A to Filter
  {7, 6, 142, 1}, // OSC B to Filter
  {7, 7, 143, 1}, // Noise to Filter
  {7, 8, 144, 1}, // Sub to Filter
  {8, 0, 148, 1}, // Macro 5 Switch
  {8, 1, 149, 1}, // Macro 6 Switch
  {8, 2, 150, 1}, // Macro 7 Switch
  {8, 3, 151, 1}, // Macro 8 Switch
  {8, 4, 152, 1}, // Macro 9 Switch
  {8, 5, 153, 1}, // Macro 10 Switch
  {8, 6, 154, 1}, // Macro 11 Switch
  {8, 7, 155, 1}, // Macro 12 Switch
  {8, 8, 156, 1}, // Macro 13 Switch
  {8, 9, 157, 1}, // Macro 14 Switch
  {8, 10, 158, 1},// Macro 15 Switch
  {8, 11, 159, 1} // Macro 16 Switch
};

// Resto del código completo

uint8_t filtro[NUM_CONTROLES][NUM_MEDIAS];
uint8_t idxFiltro[NUM_CONTROLES];
uint8_t ultimoValor[NUM_CONTROLES];

void setup() {
  pinMode(S0_PIN, OUTPUT);
  pinMode(S1_PIN, OUTPUT);
  pinMode(S2_PIN, OUTPUT);
  pinMode(S3_PIN, OUTPUT);

  // Comunicación UART bidireccional con ESP32-S3
  Serial1.begin(UART_BAUDRATE); // RX y TX conectados entre ESP32 y Arduino Micro

  memset(filtro, 0, sizeof(filtro));
  memset(idxFiltro, 0, sizeof(idxFiltro));
  memset(ultimoValor, 255, sizeof(ultimoValor));
}

void selectMux(uint8_t pin) {
  digitalWrite(S0_PIN, pin & 1);
  digitalWrite(S1_PIN, (pin >> 1) & 1);
  digitalWrite(S2_PIN, (pin >> 2) & 1);
  digitalWrite(S3_PIN, (pin >> 3) & 1);
}

uint8_t readAnalog(uint8_t mux, uint8_t pin) {
  selectMux(pin);
  return analogRead(muxPins[mux]) >> 3;  // Escala 10 bits a 7 bits (0-127)
}

void sendCC(uint8_t cc, uint8_t val, uint8_t canal) {
  // Enviar por USB MIDI
  midiEventPacket_t event = {0x0B, uint8_t(0xB0 | (canal - 1)), cc, val};
  MidiUSB.sendMIDI(event);
  MidiUSB.flush();
}

void loop() {
  // Leer y reenviar datos MIDI UART desde ESP32-S3
  while (Serial1.available() >= 3) {
    uint8_t cmd = Serial1.read();
    if ((cmd & 0xF0) == 0xB0) { // Solo reenviamos mensajes CC por ahora
      uint8_t cc = Serial1.read();
      uint8_t val = Serial1.read();
      sendCC(cc, val, (cmd & 0x0F) + 1);
    }
  }

  // Escanear potenciómetros y switches
  for (uint8_t i = 0; i < NUM_CONTROLES; i++) {
    ControlConfig cfg;
    memcpy_P(&cfg, &controles[i], sizeof(ControlConfig));

    uint8_t raw = readAnalog(cfg.mux, cfg.pin);

    filtro[i][idxFiltro[i]] = raw;
    idxFiltro[i] = (idxFiltro[i] + 1) % NUM_MEDIAS;

    uint16_t suma = filtro[i][0] + filtro[i][1];
    uint8_t media = suma / NUM_MEDIAS;

    if (abs(media - ultimoValor[i]) > 1) {
      sendCC(cfg.cc, media, cfg.canal);
      ultimoValor[i] = media;
    }
  }
  delay(3);
}
