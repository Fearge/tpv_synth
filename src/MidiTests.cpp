#include <Arduino.h>
#include "MIDI.h";
#include <synth.h>

// Create a MIDI object
MIDI_CREATE_DEFAULT_INSTANCE();

// Define the LED pin
const int ledPin = 13;  // You can choose any digital pin

//how many notes are pressed
byte notesPressed;

int voice;

synth edgar;

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Initialize MIDI communication
  pinMode(ledPin, OUTPUT);       // Set the LED pin as an output
  edgar.begin();
}

void loop() {
  // Check for incoming MIDI messages
  if (MIDI.read()) {
    byte status = MIDI.getType();  // Get the type of MIDI message

    // Check if it's a Note On message (status byte 0x90 and velocity > 0)
    if (status == 0x90 && MIDI.getData2() > 0) {
      
      //every note simultaneously pressed, takes 1 bit Space of the Byte
      notesPressed = notesPressed << 1;
      notesPressed++;

    //set voices
    switch (notesPressed)
    {
    case 0x1:
      voice = 0;
      break;

    case 0x3:
    voice = 1;
    break;

    case 0x7:
    voice = 2;
    break;

    case 0xF:
    voice = 3;
    break;
    
    default:
      break;
    }

    //trigger synth with current Note
      edgar.mTrigger(voice,MIDI.getData1());
      
    }

    // Check if it's a Note Off message (status byte 0x80 or velocity = 0)
    if ((status == 0x80) || (status == 0x90 && MIDI.getData2() == 0)) {
        //make Space in Byte
        notesPressed = notesPressed >> 1;
      
    }
  }
}
