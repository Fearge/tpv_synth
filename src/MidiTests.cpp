#include <Arduino.h>
#include "MIDI.h";

// Create a MIDI object
MIDI_CREATE_DEFAULT_INSTANCE();

// Define the LED pin
const int ledPin = 13;  // You can choose any digital pin

void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);  // Initialize MIDI communication
  pinMode(ledPin, OUTPUT);       // Set the LED pin as an output
}

void loop() {
  // Check for incoming MIDI messages
  if (MIDI.read()) {
    byte status = MIDI.getType();  // Get the type of MIDI message

    // Check if it's a Note On message (status byte 0x90 and velocity > 0)
    if (status == 0x90 && MIDI.getData2() > 0) {

        digitalWrite(ledPin, HIGH);  // Turn the LED on
      
    }

    // Check if it's a Note Off message (status byte 0x80 or velocity = 0)
    if ((status == 0x80) || (status == 0x90 && MIDI.getData2() == 0)) {
        digitalWrite(ledPin, LOW);  // Turn the LED off
      
    }
  }
}
