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
#include <NoodleSynth.h>
//#include <settings.h>
#include <MIDI.h>

int voices = 4;
int usedVoice;

MIDI_CREATE_DEFAULT_INSTANCE();
synthEngine buzzer;

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  int i;
  for (i = 0; i < voices; i++)
  { // find a polyphony instrument that isn't taken at the moment
    if ((buzzer.getChannel(i) == channel && buzzer.getNote(i) == pitch /* && i == lastUsedVoice*/) || (i != usedVoice && buzzer.getVolume(i) == 0))
    {
      break;
    }
  }
  //lastUsedVoice = i;
  buzzer.setVolume(i,127);//maxVolume
  buzzer.setNote(i, pitch);
  buzzer.setChannel(i, channel); // use idle polyphony engines
  buzzer.mute(i, 0);
  buzzer.setFrequency(i, buzzer.getNoteAsFrequency(pitch, 0x40));
  buzzer.setLength(i,128);
  buzzer.trigger(i);

}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
  for (int i = 0; i < voices; i++)
  { // find the polyphony instrument that you assigned a channel earlier in noteOn
    if ((buzzer.getChannel(i) == channel) && (buzzer.getNote(i) == pitch))
    {
      buzzer.setLength(i, 0); // ansonsten Artefakte beim loslassen
      buzzer.mute(i, 1);
    }
  }
}

void setup()
{
  // buzzer.begin(SINE, ENVELOPE1, 64);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  // buzzer.setSustain(SUSTAIN);

  for (int i = 0; i < voices; i++)
  {
    buzzer.begin(i, CHA);
    buzzer.setupVoice(i, SINE, 60, ENVELOPE0, 127, 64);
    buzzer.setChannel(i, 1);
    buzzer.mute(i,1);

  }
}

void loop()
{
  MIDI.read();
  for (int i = 0; i < 4;i++){
    if (!buzzer.isMute(i)){
      buzzer.trigger(i);
    }
  }
  // buzzer.update();
}
