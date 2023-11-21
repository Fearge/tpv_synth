#include <MIDI.h>
#include <synthEngine.h>
#include <NoodleSynth.h>
#include <settings.h>

//MIDI_CREATE_DEFAULT_INSTANCE();
//synthEngine buzzer;
//#define VOICES 4 // number of Voices the synth can play
//int usedVoice = 0;

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
    mixer.setVolume(i, 127); // maxVolume
    mixer.setNote(i, pitch);
    mixer.setChannel(i, channel); // use idle polyphony engines
    mixer.mute(i, 0);
    mixer.setFrequency(i, mixer.getNoteAsFrequency2(pitch));
    mixer.setLength(i, 128);
    // buzzer.trigger(i);
}

void handleNoteOff(byte channel, byte pitch, byte velocity)
{
    for (int i = 0; i < NUM; i++)
    { // find the polyphony instrument that you assigned a channel earlier in noteOn
        if ((mixer.getChannel(i) == channel) && (mixer.getNote(i) == pitch))
        {
            mixer.setLength(i, 0); // ansonsten Artefakte beim loslassen
            mixer.mute(i, 1);
        }
    }
}

void pitchChange(byte channel, int bend)
{
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
#if defined(pitchWheel)

#endif
}