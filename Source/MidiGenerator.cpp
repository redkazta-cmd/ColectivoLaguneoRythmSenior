#include "MidiGenerator.h"

namespace
{
    constexpr int ppq = 480;
    constexpr int barTicks = ppq * 4;

    // Escalas en notas MIDI (C4 = 60)
    const int pentatonicMajor[] = { 48, 50, 52, 55, 57, 60, 62, 64, 67, 69, 72, 74 };
    const int majorScale[]      = { 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72 };
    const int minorScale[]      = { 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72 };
}

juce::MidiFile MidiGenerator::generateMidi(int genreIndex, int bpm, juce::int64 seed)
{
    juce::MidiFile midi;
    midi.setTicksPerQuarterNote(ppq);

    juce::Random rng(seed);

    // Formato 0: todo en un solo track
    juce::MidiMessageSequence seq;

    // Meta-evento de tempo al inicio (microsegundos por pulso)
    int usPerQuarter = 60000000 / juce::jlimit(1, 300, bpm);
    seq.addEvent(juce::MidiMessage::tempoMetaEvent(usPerQuarter), 0);

    // Nombre de pista
    juce::String trackName;
    switch (genreIndex)
    {
        case 0: trackName = "Yoga Ambient"; break;
        case 1: trackName = "Meditation"; break;
        case 2: trackName = "Deep Sleep"; break;
        default: trackName = "Ambient"; break;
    }
    seq.addEvent(juce::MidiMessage::textMetaEvent(3, trackName), 0);

    const int bars = 24; // duración relajada

    switch (genreIndex)
    {
        case 0: generateYogaAmbient(seq, bars, rng); break;
        case 1: generateMeditation(seq, bars, rng); break;
        case 2: generateDeepSleep(seq, bars, rng); break;
        default: break;
    }

    seq.updateMatchedPairs();
    midi.addTrack(seq);

    return midi;
}

void MidiGenerator::addNote(juce::MidiMessageSequence& seq, int channel, int note,
                            int startTick, int durationTicks, float baseVelocity, juce::Random& rng)
{
    auto vel = static_cast<juce::uint8>(juce::jlimit(1, 127,
        static_cast<int>(baseVelocity * 127.0f * (rng.nextFloat() * 0.1f + 0.9f))));
    seq.addEvent(juce::MidiMessage::noteOn(channel, note, vel), startTick);
    seq.addEvent(juce::MidiMessage::noteOff(channel, note, vel), startTick + durationTicks);
}

// --------------- YOGA AMBIENT (Pentatónica Mayor, notas largas, espaciadas) ---------------
void MidiGenerator::generateYogaAmbient(juce::MidiMessageSequence& seq, int bars, juce::Random& rng)
{
    const int noteCount = sizeof(pentatonicMajor) / sizeof(pentatonicMajor[0]);
    int currentTick = 0;

    for (int bar = 0; bar < bars; ++bar)
    {
        // 1-2 notas por compás, largas (blancas o redondas)
        int notesInBar = rng.nextBool() ? 1 : 2;
        for (int n = 0; n < notesInBar; ++n)
        {
            int noteIdx = rng.nextInt(noteCount);
            int note = pentatonicMajor[noteIdx];
            int duration = (rng.nextFloat() < 0.6f) ? (ppq * 2) : (ppq * 4); // blanca o redonda
            float vel = rng.nextFloat() * 0.3f + 0.5f; // suave 0.5-0.8

            addNote(seq, 1, note, currentTick, duration, vel, rng);
            currentTick += duration + (rng.nextInt(3) * ppq); // pequeña pausa aleatoria
        }

        // Asegurar avance mínimo de un compás
        if (currentTick < (bar + 1) * barTicks)
            currentTick = (bar + 1) * barTicks;
    }
}

// --------------- MEDITATION (Mayor Natural, movimientos suaves, sustain) ---------------
void MidiGenerator::generateMeditation(juce::MidiMessageSequence& seq, int bars, juce::Random& rng)
{
    const int noteCount = sizeof(majorScale) / sizeof(majorScale[0]);
    int currentTick = 0;
    int prevNote = 60;

    for (int bar = 0; bar < bars; ++bar)
    {
        // 1 nota por compás, muy larga, con transiciones suaves de ±2-3 semitonos
        int delta = rng.nextInt(7) - 3; // -3 a +3
        int prevIdx = 0;
        for (int i = 0; i < noteCount; ++i)
            if (majorScale[i] == prevNote) { prevIdx = i; break; }
        int idx = juce::jlimit(0, noteCount - 1, prevIdx + delta);
        if (idx < 0 || idx >= noteCount)
            idx = rng.nextInt(noteCount);

        int note = majorScale[idx];
        int duration = (rng.nextFloat() < 0.7f) ? (ppq * 4) : (ppq * 6); // redonda o redonda con puntillo
        float vel = rng.nextFloat() * 0.2f + 0.55f;

        addNote(seq, 1, note, currentTick, duration, vel, rng);
        prevNote = note;
        currentTick += barTicks;
    }
}

// --------------- DEEP SLEEP (Menor Natural, bajas frecuencias, notas larguísimas) ---------------
void MidiGenerator::generateDeepSleep(juce::MidiMessageSequence& seq, int bars, juce::Random& rng)
{
    const int noteCount = sizeof(minorScale) / sizeof(minorScale[0]);
    int currentTick = 0;

    for (int bar = 0; bar < bars; ++bar)
    {
        // 1 nota cada 2 compases
        if (bar % 2 == 0)
        {
            int noteIdx = rng.nextInt(noteCount);
            int note = minorScale[noteIdx];
            int duration = barTicks * 2 - ppq; // casi 2 compases de sustain
            float vel = rng.nextFloat() * 0.15f + 0.4f; // muy suave

            addNote(seq, 1, note, currentTick, duration, vel, rng);
        }
        currentTick += barTicks;
    }
}