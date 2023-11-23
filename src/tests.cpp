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
#include <Noodles_Settings.h>//change voices to 4, for better performance, especially on pitchbend
#include <callbacks.h>
#include <waveSelector.h>


void setup()
{
  midibench.begin(MIDI_CHANNEL_OMNI);
  midibench.setHandleNoteOn(handleNoteOn);
  midibench.setHandleNoteOff(handleNoteOff);
  midibench.setHandlePitchBend(pitchChange);


  for (int i = 0; i < NUM; i++)
  {
    mixer.begin(i, CHA);
    mixer.setupVoice(i, SAW, 60, ENVELOPE1, 127, 64);
    mixer.setChannel(i, 1);
    mute(i,1);

  }
}

void loop()
{
  midibench.read();
  currentWave();
    for (int i = 0; i < NUM; i++){
    if (!isMute(i)){
      mixer.trigger(i);
    }
  }
}
