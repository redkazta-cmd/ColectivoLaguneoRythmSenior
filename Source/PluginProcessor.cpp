#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MidiGenerator.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties())
{
}

PluginProcessor::~PluginProcessor() {}

void PluginProcessor::prepareToPlay(double, int) {}
void PluginProcessor::releaseResources() {}
void PluginProcessor::processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) {}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor(*this);
}

bool PluginProcessor::hasEditor() const { return true; }

const juce::String PluginProcessor::getName() const
{
    return "Colectivo Laguneo RhythmSenior";
}

bool PluginProcessor::acceptsMidi() const { return true; }
bool PluginProcessor::producesMidi() const { return true; }
double PluginProcessor::getTailLengthSeconds() const { return 0.0; }

int PluginProcessor::getCurrentBpm() const
{
    static const int bpms[] = { 60, 70, 80, 90, 100 };
    auto idx = juce::jlimit(0, 4, currentBpmIndex);
    return bpms[idx];
}

int PluginProcessor::getNumPrograms() { return 1; }
int PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram(int) {}
const juce::String PluginProcessor::getProgramName(int) { return {}; }
void PluginProcessor::changeProgramName(int, const juce::String&) {}

void PluginProcessor::getStateInformation(juce::MemoryBlock&) {}
void PluginProcessor::setStateInformation(const void*, int) {}

juce::File PluginProcessor::generateMidiFile(int genreIndex, int bpm)
{
    auto seed = juce::Time::getMillisecondCounter();
    auto midi = MidiGenerator::generateMidi(genreIndex, bpm, seed);
    tempFile = std::make_unique<juce::TemporaryFile>(".mid");
    juce::FileOutputStream out(tempFile->getFile());
    if (out.openedOk())
    {
        midi.writeTo(out);
        return tempFile->getFile();
    }
    tempFile.reset();
    return {};
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}