#include <Arduino.h>
#include <MIDI.h>
#include <MidiKey.h>
#include <MIDINotes.h>

#ifdef UPPER_KEYS
#define NUM_KEYS 37
const midikey keys[NUM_KEYS] = {
  {2, F5},
  {3, Fs5},
  {4, G5},
  {5, Gs5},
  {6, A5},
  {7, As5},
  {8, B5},
  {9, C6},
  {10, Cs6},
  {11, D6},
  {12, Ds6},
  {13, E6},
  {14, F6},
  {15, Fs6},
  {16, G6},
  {17, Gs6},
  {18, A6},
  {19, As6},
  {20, B6},
  {21, C7},
  {22, Cs7},
  {23, D7},
  {24, Ds7},
  {25, E7},
  {26, F7},
  {27, Fs7},
  {28, G7},
  {29, Gs7},
  {30, A7},
  {31, As7},
  {32, B7},
  {33, C8},
  {34, Cs8},
  {35, D8},
  {36, Ds8},
  {37, E8},
  {38, F8}
};
#endif
#ifdef LOWER_KEYS
#define NUM_KEYS 37
const midikey keys[NUM_KEYS] = {
  {2, F2},
  {3, Fs2},
  {4, G2},
  {5, Gs2},
  {6, A2},
  {7, As2},
  {8, B2},
  {9, C3},
  {10, Cs3},
  {11, D3},
  {12, Ds3},
  {13, E3},
  {14, F3},
  {15, Fs3},
  {16, G3},
  {17, Gs3},
  {18, A3},
  {19, As3},
  {20, B3},
  {21, C4},
  {22, Cs4},
  {23, D4},
  {24, Ds4},
  {25, E4},
  {26, F4},
  {27, Fs4},
  {28, G4},
  {29, Gs4},
  {30, A4},
  {31, As4},
  {32, B4},
  {33, C5},
  {34, Cs5},
  {35, D5},
  {36, Ds5},
  {37, E5},
  {38, F5}
};
#endif
#ifdef BASS_KEYS // bass keys
NUM_KEYS=13
const midikey keys[NUM_KEYS] = {
  {2, C1},
  {3, Cs1},
  {4, D1},
  {5, Ds1},
  {6, E1},
  {7, F1},
  {8, Fs1},
  {9, G1},
  {10, Gs1},
  {11, A1},
  {12, As1},
  {13, B1},
  {14, C2}
}
#endif

uint32_t pressedKeys = 0x00000000;
uint32_t previousKeys = 0x00000000;

int velocity = 0;
int channel = 1;

MIDI_CREATE_DEFAULT_INSTANCE();

void readKeys()
{
  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    if(digitalRead(keys[i].pin) == LOW)
    {
      bitWrite(pressedKeys, i, 1);
      delay(50);
    }
    else
    {
      bitWrite(pressedKeys, i, 0);
    }
  }
}

void playNotes()
{
  for (uint8_t i = 0; i < NUM_KEYS; i++)
  {
    if (bitRead(pressedKeys, i) != bitRead(previousKeys, i))
    {
      if (bitRead(pressedKeys, i))
      {
        Serial.println("on - " + keys[i].note);
        MIDI.sendNoteOn(keys[i].note, velocity, channel);
      }
      else
      {
        Serial.println("off - " + keys[i].note);
        MIDI.sendNoteOff(keys[i].note, velocity, channel);
      }
    }
  }
  previousKeys = pressedKeys;
  pressedKeys = 0x00000000;
}

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOn();
  for(uint8_t i = 0; i < NUM_KEYS; i++)
  {
    pinMode(keys[i].pin, INPUT_PULLUP);
  }
}

void loop() {
  MIDI.read();
  readKeys();
  playNotes();
}
