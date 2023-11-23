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

#define PAD 0 //Mode for playing long Pads
#define PLUCK 1 //mode for playing short plucks

//Variables for Waveform Selector
const int wavePin = 12;
int waveState = 0;        // current state of the button
int lastWave = 0;    // previous state of the button
int selectedWave = SINE;

//Variables for Mode Selector
const int modePin = 2;
int modeState = 0;
int lastMode;
int startMode = PAD;

int currentWave(){
  // read the pushbutton input pin:
  waveState = digitalRead(wavePin);

  // compare the buttonState to its previous state
  if (waveState != lastWave) {
    // if the state has changed, increment the counter
    switch (waveState)
    {
    case HIGH:
        if(selectedWave == SINE){
        selectedWave = SQUARE;
      }else selectedWave = SINE;
        break;
    
    default:
        break;
    }
  // save the current state as the last state, for next time through the loop
  lastWave = waveState;
  } return selectedWave;
}

bool isPluck(){
    modeState = digitalRead(modePin);
    bool hasChanged;

  // compare the buttonState to its previous state
  if (modeState != lastMode) {
    // if the state has changed, increment the counter
    switch (modeState)
    {
    case HIGH:
        if(modeState == PAD){
        hasChanged = 1;
      }else hasChanged = 0;
        break;
    default:
        break;
    }
  // save the current state as the last state, for next time through the loop
  modeState = lastMode;
  } return hasChanged;
}