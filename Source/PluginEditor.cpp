#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p), processor(p)
{
    setSize(350, 200);
    setLookAndFeel(&lookAndFeel);
    getLookAndFeel().setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff1a1a1a));

    addAndMakeVisible(genreSelector);
    genreSelector.addItem("Trap", 1);
    genreSelector.addItem("Crank", 2);
    genreSelector.addItem("Reggaetón", 3);
    genreSelector.addItem("Boom Bap", 4);
    genreSelector.onChange = [this] { processor.setCurrentGenreIndex(genreSelector.getSelectedId() - 1); };
    genreSelector.setBounds(10, 10, 200, 25);

    addAndMakeVisible(generateButton);
    generateButton.setButtonText("GENERATE");
    generateButton.onClick = [this] {
        juce::File midiFile = processor.generateMidiFile(processor.getCurrentGenreIndex());
        if (midiFile.existsAsFile())
        {
            dragArea.setMidiFile(midiFile, getGenreName(processor.getCurrentGenreIndex()));
        }
        else
        {
            dragArea.clear();
        }
    };
    generateButton.setBounds(10, 50, 150, 25);

    addAndMakeVisible(dragArea);
    dragArea.setBounds(10, 90, 330, 80);
}

PluginEditor::~PluginEditor()
{
    setLookAndFeel(nullptr);
}

juce::String PluginEditor::getGenreName(int index) const
{
    switch (index)
    {
        case 0: return "Trap";
        case 1: return "Crank";
        case 2: return "Reggaeton";
        case 3: return "Boom Bap";
        default: return "Unknown";
    }
}