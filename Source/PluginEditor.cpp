#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(350, 200);
    setLookAndFeel(&lookAndFeel);
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1a1a1a));

    addAndMakeVisible(genreSelector);
    genreSelector.addItem("Yoga Ambient", 1);
    genreSelector.addItem("Meditation", 2);
    genreSelector.addItem("Deep Sleep", 3);
    genreSelector.onChange = [this] { processor.setCurrentGenreIndex(genreSelector.getSelectedId() - 1); };
    genreSelector.setBounds(10, 10, 160, 25);

    addAndMakeVisible(bpmSelector);
    bpmSelector.addItem("60 BPM", 1);
    bpmSelector.addItem("70 BPM", 2);
    bpmSelector.addItem("80 BPM", 3);
    bpmSelector.addItem("90 BPM", 4);
    bpmSelector.addItem("100 BPM", 5);
    bpmSelector.onChange = [this] { processor.setCurrentBpmIndex(bpmSelector.getSelectedId() - 1); };
    bpmSelector.setBounds(180, 10, 100, 25);

    addAndMakeVisible(generateButton);
    generateButton.setButtonText("GENERATE");
    generateButton.onClick = [this] {
        juce::File midiFile = processor.generateMidiFile(processor.getCurrentGenreIndex(), processor.getCurrentBpm());
        if (midiFile.existsAsFile())
        {
            dragArea.setMidiFile(midiFile, getGenreName(processor.getCurrentGenreIndex()));
        }
        else
        {
            dragArea.clear();
        }
    };
    generateButton.setBounds(10, 45, 270, 25);

    addAndMakeVisible(dragArea);
    dragArea.setBounds(10, 80, 330, 100);
}

PluginEditor::~PluginEditor()
{
    setLookAndFeel(nullptr);
}

juce::String PluginEditor::getGenreName(int index) const
{
    switch (index)
    {
        case 0: return "Yoga Ambient";
        case 1: return "Meditation";
        case 2: return "Deep Sleep";
        default: return "Unknown";
    }
}