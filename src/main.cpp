#include <Arduino.h>
#include <MIDI.h>
#include <MidiKey.h>
#include <MIDINotes.h>

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

uint32_t pressedKeys = 0x00000000;
uint32_t previousKeys = 0x00000000;

int velocity = 0;
int channel = 1;

MIDI_CREATE_DEFAULT_INSTANCE();

void readButtons()
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
        MIDI.sendNoteOn(keys[i].note, velocity, channel);
      }
      else
      {
        MIDI.sendNoteOff(keys[i].note, velocity, channel);
      }
    }
  }
  previousKeys = pressedKeys;
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
  readButtons();
  playNotes();
}
