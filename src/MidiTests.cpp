#include <Arduino.h>
#include "MIDI.h"
#include <synth.h>

#define NOTE_C4 37
#define NOTE_CS4 38
#define NOTE_D4 39
#define NOTE_DS4 40
#define NOTE_E4 41
#define NOTE_F4 42
#define NOTE_FS4 43
#define NOTE_G4 44
#define NOTE_GS4 45
#define NOTE_A4 46
#define NOTE_AS4 47
#define NOTE_B4 48
#include <synth.h>

// Create a MIDI object
MIDI_CREATE_DEFAULT_INSTANCE();
byte notes;
int note1;
int note2;
int note3;
int note4;

synth edgar;

// Define the LED pin
const int ledPin = 13; // You can choose any digital pin

void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize MIDI communication
  edgar.begin();
  // set Voices
  edgar.setupVoice(0, SAW, 0, ENVELOPE0, 80, 64);
  edgar.setupVoice(1, SAW, 0, ENVELOPE0, 80, 64);
  edgar.setupVoice(2, SAW, 0, ENVELOPE0, 80, 64);
  edgar.setupVoice(3, SAW, 0, ENVELOPE0, 80, 64);

  pinMode(ledPin, OUTPUT); // Set the LED pin as an output
}

void loop()
{
  // Check for incoming MIDI messages
  if (MIDI.read())
  {
    byte status = MIDI.getType(); // Get the type of MIDI message

    // Check if it's a Note On message (status byte 0x90 and velocity > 0)
    if (status == 0x90 && MIDI.getData2() > 0)
    {
      // check if all voices are playing
      notes = notes << 1;
      notes++;
      switch (notes)
      {
      case 0x01:
        note1 = MIDI.getData1();
        edgar.mTrigger(0, note1);
        digitalWrite(ledPin, HIGH);
        break;

      case 0x03:
        note2 = MIDI.getData1();
        edgar.mTrigger(1, note2);
        digitalWrite(ledPin, HIGH);
        break;

      case 0x07:
        note3 = MIDI.getData1();
        edgar.mTrigger(2, note3);
        break;

      case 0x0F:
        note4 = MIDI.getData1();
        edgar.mTrigger(3, note4);
        break;

      default:
        break;
      }
    }

    // Check if it's a Note Off message (status byte 0x80 or velocity = 0)
    if ((status == 0x80) || (status == 0x90 && MIDI.getData2() == 0))
    {
      notes = notes >> 1;

      int currNote = MIDI.getData1();
      if (currNote == note1)
      {
        digitalWrite(ledPin, LOW); // Turn the LED on
      }
      else if (currNote == note2)
      {
        digitalWrite(52, LOW); // Turn the LED on
      }
      else if (currNote == note3)
      {
        digitalWrite(51, LOW); // Turn the LED on
      }
      else if (currNote == note4)
      {
        digitalWrite(51, LOW); // Turn the LED on
      }
    }
  }
}
