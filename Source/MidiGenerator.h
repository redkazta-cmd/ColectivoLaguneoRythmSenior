#pragma once
#include <juce_audio_basics/juce_audio_basics.h>

class MidiGenerator
{
public:
    static juce::MidiFile generateMidi(int genreIndex, juce::int64 seed);

private:
    static void addNote(juce::MidiMessageSequence& seq, int channel, int note,
                        int startTick, int durationTicks, float baseVelocity, juce::Random& rng);

    static void generateTrap(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                             juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng);
    static void generateCrank(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                              juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng);
    static void generateReggaeton(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                  juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng);
    static void generateBoomBap(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng);
};