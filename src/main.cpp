#include <Arduino.h>
#include <MIDI.h>
#include <MidiKey.h>
#include <MIDINotes.h>

#define NUM_KEYS 13



const midikey keys[NUM_KEYS] = {
  {2, F6},
  {3, Fs6},
  {4, G6},
  {5, Gs6},
  {6, A6},
  {7, As6},
  {8, B6},
  {9, C7},
  {10, Cs7},
  {11, D7},
  {12, Ds7},
  {13, E7},
  {14, F7}
};

uint32_t pressedKeys = 0x00000000;
uint32_t previousKeys = 0x00000000;

const int velocity = 0;

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
        MIDI.sendNoteOn(keys[i].note, velocity, 1);
      }
      else
      {
        MIDI.sendNoteOff(keys[i].note, velocity, 1);
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
