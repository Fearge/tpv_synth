#include <MIDI.h>
#include <synthEngine.h>
#include <NoodleSynth.h>
#include <settings.h> //here: change sample rate to 20k for smoother sounds
#include <Noodles_Settings.h>

int waveform = 0;

static bool isItMute[maxVOICES];
bool isMute(unsigned char voice)
{
    return isItMute[voice];
}

void mute(unsigned char voice, bool m)
{
    isItMute[voice] = m;
}

void handleNoteOn(byte channel, byte pitch, byte velocity)
{
    int i;
    for (i = 0; i < NUM; i++)
    { // find a polyphony instrument that isn't taken at the moment
        if ((mixer.getChannel(i) == channel && mixer.getNote(i) == pitch /* && i == lastUsedVoice*/) || (i != lastUsedVoice && mixer.getVolume(i) == 0))
        {
            break;
        }
    }
    lastUsedVoice = i;

    // set parameter
    mixer.setVolume(i, 127);
    mixer.setNote(i, pitch);
    mixer.setChannel(i, channel); // use idle polyphony engines
    mixer.setWave(i, waveform);
    mute(i, 0);
    float freq = synthEngine::getNoteAsFrequency(pitch, tempData[mixer.getChannel(i)].bendFactor);
    mixer.setFrequency(i, freq);
    mixer.setLength(i, 128);
    mixer.trigger(i);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    for (int i = 0; i < NUM; i++)
    { // find the polyphony instrument that you assigned a channel earlier in noteOn
        if ((mixer.getChannel(i) == channel) && (mixer.getNote(i) == pitch))
        {
            mixer.setLength(i, 15); // ansonsten Artefakte beim loslassen
            mute(i, 1);
        }
    }
}

void pitchChange(byte channel, int bend)
{
#if defined(pitchWheel)
    // no bend
    if (bend == 0x40)
    {
        // Do Nothing
    }
    int bendFactor = map(bend, -8192, 8192, -4096, 4096);
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