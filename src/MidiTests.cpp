#include <Arduino.h>
#include "MIDI.h"
#include <NoodleSynth.h>

// Create a MIDI object
MIDI_CREATE_DEFAULT_INSTANCE();
byte notes;
int note1;
int note2;
int note3;
int note4;

//waveform of synth
char waveform = SINE;

//envelope of synth
char envelope = ENVELOPE3;

synthEngine edgar;

// Define the LED pin
const int ledPin = 13; // You can choose any digital pin

void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize MIDI communication
  edgar.begin(CHA,);
  // set Voices
  edgar.setupVoice(0, waveform, 0, envelope, 80, 64);
  edgar.setupVoice(1, waveform, 0, envelope, 80, 64);
  edgar.setupVoice(2, waveform, 0, envelope, 80, 64);
  edgar.setupVoice(3, waveform, 0, envelope, 80, 64);

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
      // how many voices are playing
      notes = notes << 1;
      notes++;
      switch (notes)
      {
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
      note1 = MIDI.getData1();
        edgar.mTrigger(0, note1);
        digitalWrite(ledPin, HIGH);
        break;
      }
    }

    // Check if it's a Note Off message (status byte 0x80 or velocity = 0)
    if ((status == 0x80) || (status == 0x90 && MIDI.getData2() == 0))
    {
      notes = notes >> 1;

      int currentNote = MIDI.getData1();
      if (currentNote == note1)
      {
        //TODO: Trigger afterNoteEnvelope
        digitalWrite(ledPin, LOW); // Turn the LED off
      }
      else if (currentNote == note2)
      {
        digitalWrite(ledPin, LOW); // Turn the LED on
      }
      else if (currentNote == note3)
      {
        digitalWrite(ledPin, LOW); // Turn the LED on
      }
      else if (currentNote == note4)
      {
        digitalWrite(ledPin, LOW); // Turn the LED on
      }
    }
  }
}
