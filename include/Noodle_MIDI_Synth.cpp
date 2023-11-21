/*
   Noodle Synth was made by Nathan Ramanathan.
   GNU license 2019
*/
/////////////////////////////////////////////////////////----Libraries-------//////////////////////////////////////////////////////////////////
#define NoodleMidiSynth
#include <NoodleSynth.h> //contains list of this libraries dependencies
#include <MIDI.h>
#include "settings.h"
#include "MIDI_callbacks.h"
/////////////////////////////////////-------SETUP-------////////////////////////////////////////////////////
void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(buttonPin, INPUT);

#if defined(__arm__) && defined(TEENSYDUINO) && defined(USB_MIDI_MODE)
  usbMIDI.setHandleNoteOff(handleNoteOff);
  usbMIDI.setHandleNoteOn(handleNoteOn);
  usbMIDI.setHandleControlChange(myControlChange);
  usbMIDI.setHandleProgramChange(instrumentChange);
  usbMIDI.setHandlePitchChange(pitchChange);
#endif

  midibench.setHandleNoteOff(handleNoteOff);
  midibench.setHandleNoteOn(handleNoteOn);
  // midibench.setHandleControlChange(myControlChange);
  // midibench.setHandleProgramChange(instrumentChange);
  // midibench.setHandlePitchBend(pitchChange);
  midibench.begin(MIDI_CHANNEL_OMNI);

  for (int i = 0; i < NUM; i++)
  {
    mixer.begin(i, pins[i % (sizeof(pins) / sizeof(pins[0]))]);
    mixer.setupVoice(i, SINE, 60, ENVELOPE0, 127, 64);
    mixer.setVolume(i, 127);
#if defined(SUSTAIN_TYPE)
    mixer.setSustain(i, SUSTAIN_TYPE);
#endif
    mixer.setChannel(i, 1);
  }
#if defined(__AVR_ATmega2560__) || defined(__arm__) && defined(TEENSYDUINO)
  Serial1.begin(31250);
  Serial.begin(115200);
#if defined(DEBUG)
  Serial.println("Done Setting up");
#endif
#else
#if defined(HAIRLESS_MIDI)
  Serial.begin(38400);
#else
  Serial.begin(31250);
#endif
#endif
}
/////////////////////////////////////-------LOOP-------////////////////////////////////////////////////////
void loop()
{
  //currentWave();
  midibench.read();
  for (int i; i < 4; i++)
  {
    if (!mixer.isMute(i))
    {
      mixer.trigger(i);
    }
  }
}
