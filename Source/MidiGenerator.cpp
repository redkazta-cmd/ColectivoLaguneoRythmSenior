#include "MidiGenerator.h"

juce::MidiFile MidiGenerator::generateMidi(int genreIndex, juce::int64 seed)
{
    juce::MidiFile midi;
    midi.setTicksPerQuarterNote(480);

    juce::Random rng(seed);

    juce::MidiMessageSequence kickSeq, snareSeq, hihatSeq, bassSeq, percSeq;

    // Nombres de pista mediante meta‑eventos de texto (tipo 3)
    kickSeq.addEvent(juce::MidiMessage::textMetaEvent(3, "Kick"), 0);
    snareSeq.addEvent(juce::MidiMessage::textMetaEvent(3, "Snare"), 0);
    hihatSeq.addEvent(juce::MidiMessage::textMetaEvent(3, "HiHat"), 0);
    bassSeq.addEvent(juce::MidiMessage::textMetaEvent(3, "808"), 0);
    percSeq.addEvent(juce::MidiMessage::textMetaEvent(3, "Percussion"), 0);

    switch (genreIndex)
    {
        case 0: generateTrap(kickSeq, snareSeq, hihatSeq, bassSeq, rng); break;
        case 1: generateCrank(kickSeq, snareSeq, hihatSeq, bassSeq, rng); break;
        case 2: generateReggaeton(kickSeq, snareSeq, hihatSeq, bassSeq, rng); break;
        case 3: generateBoomBap(kickSeq, snareSeq, hihatSeq, bassSeq, rng); break;
        default: break;
    }

    midi.addTrack(kickSeq);
    midi.addTrack(snareSeq);
    midi.addTrack(hihatSeq);
    midi.addTrack(bassSeq);
    midi.addTrack(percSeq);

    return midi;
}

void MidiGenerator::addNote(juce::MidiMessageSequence& seq, int channel, int note,
                            int startTick, int durationTicks, float baseVelocity, juce::Random& rng)
{
    auto vel = static_cast<juce::uint8>(juce::jlimit(1, 127,
        static_cast<int>(baseVelocity * 127.0f * (rng.nextFloat() * 0.15f + 0.85f))));
    seq.addEvent(juce::MidiMessage::noteOn(channel, note, vel), startTick);
    seq.addEvent(juce::MidiMessage::noteOff(channel, note, vel), startTick + durationTicks);
}

// --------------- TRAP ---------------
void MidiGenerator::generateTrap(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                 juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng)
{
    const int barTicks = 1920;
    for (int bar = 0; bar < 32; ++bar)
    {
        int offset = bar * barTicks;
        addNote(kick, 10, 36, offset,         240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 1920,  240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 1440,  120, 0.7f, rng);
        addNote(snare, 10, 38, offset + 960,   240, 1.0f, rng);
        addNote(snare, 10, 38, offset + 2880,  240, 1.0f, rng);
        for (int i = 0; i < 8; ++i)
        {
            int t = offset + i * 480;
            if (rng.nextFloat() < 0.15f)
                for (int j = 0; j < 4; ++j)
                    addNote(hihat, 10, 42, t + j * 120, 60, 1.0f, rng);
            else
                addNote(hihat, 10, 42, t, 240, 1.0f, rng);
        }
        bool latigazo = (bar % 2 == 0) && (rng.nextFloat() < 0.4f);
        if (!latigazo)
        {
            addNote(bass, 3, 48, offset,         1920, 1.0f, rng);
            addNote(bass, 3, 48, offset + 1920,  1920, 1.0f, rng);
        }
        else
        {
            addNote(bass, 3, 48, offset,         1920, 1.0f, rng);
            addNote(bass, 3, 60, offset + 1920,  1920, 1.0f, rng);
            addNote(kick, 10, 36, offset + 1920,  120, 1.0f, rng);
        }
    }
}

// --------------- CRANK ---------------
void MidiGenerator::generateCrank(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                  juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng)
{
    const int barTicks = 1920;
    for (int bar = 0; bar < 32; ++bar)
    {
        int offset = bar * barTicks;
        for (int i = 0; i < 8; ++i)
        {
            float vel = (i == 0 || i == 4) ? 1.0f : 0.8f;
            addNote(kick, 10, 36, offset + i * 480, 240, vel, rng);
        }
        addNote(snare, 10, 38, offset + 960,   240, 1.0f, rng);
        addNote(snare, 10, 38, offset + 2880,  240, 1.0f, rng);
        for (int step = 0; step < 16; ++step)
        {
            int t = offset + step * 240;
            if (rng.nextFloat() < 0.3f)
                for (int j = 0; j < 16; ++j)
                    addNote(hihat, 10, 42, t + j * 30, 15, 1.0f, rng);
            else
                addNote(hihat, 10, 42, t, 120, 1.0f, rng);
        }
        for (int beat = 0; beat < 4; ++beat)
        {
            int note = (beat % 2 == 0) ? 48 : 60;
            addNote(bass, 3, note, offset + beat * 480, 480, 1.0f, rng);
        }
    }
}

// --------------- REGGAETÓN ---------------
void MidiGenerator::generateReggaeton(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                     juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng)
{
    const int barTicks = 1920;
    for (int bar = 0; bar < 32; ++bar)
    {
        int offset = bar * barTicks;
        addNote(kick, 10, 36, offset,         240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 1920,  240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 1440,  120, 0.6f, rng);
        addNote(kick, 10, 36, offset + 3360,  120, 0.6f, rng);
        addNote(snare, 10, 38, offset + 960,   240, 1.0f, rng);
        addNote(snare, 10, 38, offset + 2880,  240, 1.0f, rng);
        for (int step = 0; step < 16; ++step)
            addNote(hihat, 10, 42, offset + step * 240, 120, 1.0f, rng);
        if (bar % 2 == 0)
        {
            addNote(bass, 3, 48, offset,        480, 1.0f, rng);
            addNote(bass, 3, 51, offset + 480,  240, 1.0f, rng);
            addNote(bass, 3, 48, offset + 960,  480, 1.0f, rng);
            addNote(bass, 3, 51, offset + 2400, 240, 1.0f, rng);
            addNote(bass, 3, 48, offset + 2880, 480, 1.0f, rng);
        }
    }
}

// --------------- BOOM BAP ---------------
void MidiGenerator::generateBoomBap(juce::MidiMessageSequence& kick, juce::MidiMessageSequence& snare,
                                    juce::MidiMessageSequence& hihat, juce::MidiMessageSequence& bass, juce::Random& rng)
{
    const int barTicks = 1920;
    for (int bar = 0; bar < 32; ++bar)
    {
        int offset = bar * barTicks;
        addNote(kick, 10, 36, offset,         240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 1440,  120, 0.7f, rng);
        addNote(kick, 10, 36, offset + 1920,  240, 1.0f, rng);
        addNote(kick, 10, 36, offset + 3360,  120, 0.7f, rng);
        addNote(snare, 10, 38, offset + 960,   240, 1.0f, rng);
        addNote(snare, 10, 38, offset + 2880,  240, 1.0f, rng);
        for (int i = 0; i < 8; ++i)
            addNote(hihat, 10, 42, offset + i * 480, 240, 1.0f, rng);
        if (bar % 2 == 0)
            addNote(bass, 3, 48, offset, 3840, 1.0f, rng);
    }
}