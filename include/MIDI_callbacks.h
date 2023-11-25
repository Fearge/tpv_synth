/////////////////////////////////////////--------------MIDI callbacks-----------/////////////////////////////////////////////////
#include <settings.h>
/**/
void handleNoteOn(byte channel, byte pitch, byte velocity)
{
  digitalWrite(LED, HIGH);
  int i;
  for (i = 0; i < NUM; i++)
  { // find a polyphony instrument that isn't taken at the moment
    if ((mixer.getChannel(i) == channel && mixer.getNote(i) == pitch /* && i == lastUsedVoice*/) || (i != lastUsedVoice && mixer.getVolume(i) == 0))
    {
      break;
    }
  }
  lastUsedVoice = i;
  mixer.setChannel(i, channel); // use idle polyphony engines
  mixer.setNote(i, pitch);
  float freq = synthEngine::getNoteAsFrequency(pitch, 0x40);
  mixer.setFrequency(i, freq);
  // mixer.setLength(i, 128);
  mixer.mute(i, 0);
  mixer.setVolume(i, masterVolume);
  mixer.trigger(i);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
#if defined(DEBUG)
  Serial.print("NOTE off\t");
  Serial.print("channel: ");
  Serial.print(channel);
  Serial.print(", pitch=");
  Serial.print(pitch);
  Serial.print(", velocity=");
  Serial.println(velocity);
#endif
  digitalWrite(13, LOW);
  int i;
  for (i = 0; i < NUM; i++)
  { // find the polyphony instrument that you assigned a channel earlier in noteOn
    if ((mixer.getChannel(i) == channel) && (mixer.getNote(i) == pitch))
    {
      if (mixer.getSustain(i) == REV_SUSTAIN)
      {
        if (!mixer.isPedalSustain(i))
        { // if the pedal is being held, then don't retrigger note
          // mixer.trigger(i);
          mixer.setLength(i, 0);
          mixer.mute(i, 1);
        }
      }
      else
      {
        mixer.mute(i, 1);
        mixer.setLength(i, 16);
      }
      break;
    }
  }
}
void percussion(byte availableVoice, byte percussionType, byte velocity)
{
  //  Serial.print("Voice available: ");
  //  Serial.print(availableVoice);
  //  Serial.print(", Percussion: ");
  //  Serial.println(percussionType);

  //  switch (percussionType) {
  //    case 36:  //bass drum
  //      mixer.setWave(availableVoice, SINE);
  //      mixer.mTrigger(availableVoice, 82);  // instrument, note
  //      mixer.setMod(availableVoice, 33);
  //      mixer.setLength(availableVoice, 64);
  //      break;
  //    case 38: //snare with cymbal
  //      mixer.setWave(availableVoice, NOISE);
  //      mixer.mTrigger(availableVoice, 68);  // instrument, note
  //      mixer.setMod(availableVoice, 33);
  //      mixer.setLength(availableVoice, 69);
  //      break;
  //  }
}
void myControlChange(byte channel, byte control, byte value)
{
#if defined(DEBUG)
  Serial.print("channel: ");
  Serial.print(channel);
  Serial.print(", control=");
  Serial.print(control);
  Serial.print(", value=");
  Serial.println(value);
#endif

  switch (control)
  {
  case 0: // new song incoming.  Unfortunately this doesn't work sometimes :(
    if (value == 0)
    {
      for (int c = 0; c < NUM; c++)
      { // update all instruments that are on the same channel
        if (mixer.getChannel(c) == channel)
        {
          tempData[channel].isPedalSustain = false;
          tempData[channel].bendFactor = 0;
          tempData[channel].currentVolume = 127;
#if defined(SPECIFIC_WAVE_ONLY)
          tempData[channel].waveForm = SPECIFIC_WAVE_ONLY;
#else
          tempData[channel].waveForm = RAMP;
#endif
          mixer.setPedalSustain(c, tempData[channel].isPedalSustain);
          mixer.setVolume(c, tempData[channel].currentVolume);
          mixer.setWave(c, tempData[channel].waveForm);
        }
      }
    }
    break;

  case 7: // volume
    tempData[channel].currentVolume = value;
    for (int i = 0; i < NUM; i++)
    {
      if (mixer.getChannel(i) == channel)
        mixer.setVolume(i, value + 15);
    }
    break;
  case 64: // pedal Sustain
#if defined(pedalSustain)
    tempData[channel].isPedalSustain = value;
    for (int i = 0; i < NUM; i++)
    {
      if (mixer.getChannel(i) + 1 == channel)
      {
        if (!value)
        {
          if (SUSTAIN_TYPE == REV_SUSTAIN)
          {
            mixer.trigger(i);
            mixer.setLength(i, 0);
          }
          else
          {
            mixer.setLength(i, 30);
          }
        }
      }
    }
#endif
    break;
  }
}

void instrumentChange(byte channel, byte instrument)
{
#if defined(DEBUG)
  Serial.print("channel: ");
  Serial.print(channel);
  Serial.print(", instrument= ");
  Serial.println(instrument);
#endif
#if !defined(SPECIFIC_WAVE_ONLY)
  tempData[channel].waveForm = waveForms[instrument % (sizeof(waveForms) / sizeof(waveForms[0]))];
  for (int i = 0; i < NUM; i++)
  {
    if (mixer.getChannel(i) == channel)
    {
      mixer.setWave(i, tempData[channel].waveForm);
    }
  }
#else
  tempData[channel].waveForm = SPECIFIC_WAVE_ONLY;
  for (int i = 0; i < NUM; i++)
  {
    if (mixer.getChannel(i) == channel)
    {
      mixer.setWave(i, SPECIFIC_WAVE_ONLY);
    }
  }
#endif
}

void pitchChange(byte channel, int bend)
{
  int bendFactor = map(bend, -8192, 8192, -4096, 4096);
#if defined(DEBUG)
  Serial.print("channel: ");
  Serial.print(channel);
  Serial.print(", pitch bend= ");
  Serial.println(bendFactor);
#endif
#if defined(pitchWheel)
  for (int i = 0; i < NUM; i++)
  {
    if (mixer.getChannel(i) == channel)
    {
      tempData[channel].bendFactor = bendFactor;
      float freq = synthEngine::getNoteAsFrequency(mixer.getNote(i), bendFactor);
      mixer.setFrequency(i, freq);
    }
  }
#endif
}
