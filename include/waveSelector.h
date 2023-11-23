/*
  State change detection (edge detection)

  Often, you don't need to know the state of a digital input all the time, but
  you just need to know when the input changes from one state to another.
  For example, you want to know when a button goes from OFF to ON. This is called
  state change detection, or edge detection.

  This example shows how to detect when a button or button changes from off to on
  and on to off.

  The circuit:
  - pushbutton attached to pin 2 from +5V
  - 10 kilohm resistor attached to pin 2 from ground
  - LED attached from pin 13 to ground through 220 ohm resistor (or use the
    built-in LED on most Arduino boards)

  created  27 Sep 2005
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/StateChangeDetection
*/
#include <Arduino.h>
#include <synthEngine.h>
// this constant won't change:
const int buttonPin = 2;  // the pin that the pushbutton is attached to

// Variables will change:
int buttonPushCounter = 0;  // counter for the number of button presses
int buttonState = 0;        // current state of the button
int lastButtonState = 0;    // previous state of the button
int selectedWave = SINE;

int currentWave(){
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    switch (buttonState)
    {
    case HIGH:
        if(selectedWave == SINE){
        selectedWave = SQUARE;
      }else selectedWave = SINE;
        /* code */
        break;
    
    default:
        break;
    }
  // save the current state as the last state, for next time through the loop
  lastButtonState = buttonState;
  } return selectedWave;
}