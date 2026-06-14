#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class MidiGenerator
{
public:
    static juce::MidiFile generateMidi(int genreIndex, int bpm, juce::int64 seed);

private:
    static void addNote(juce::MidiMessageSequence& seq, int channel, int note,
                        int startTick, int durationTicks, float baseVelocity, juce::Random& rng);

    static void generateYogaAmbient(juce::MidiMessageSequence& seq, int bars, juce::Random& rng);
    static void generateMeditation(juce::MidiMessageSequence& seq, int bars, juce::Random& rng);
    static void generateDeepSleep(juce::MidiMessageSequence& seq, int bars, juce::Random& rng);
};