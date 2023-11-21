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
//#include <settings.h>
#include <callbacks.h>

void setup()
{
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandlePitchBend(pitchChange);


  for (int i = 0; i < NUM; i++)
  {
    mixer.begin(i, CHA);
    mixer.setupVoice(i, SINE, 60, ENVELOPE0, 127, 64);
    mixer.setChannel(i, 1);
    mixer.mute(i,1);

  }
}

void loop()
{
  MIDI.read();
    for (int i = 0; i < NUM; i++){
    if (!mixer.isMute(i)){
      mixer.trigger(i);
    }
  }

  // buzzer.update();
}
