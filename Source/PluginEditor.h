#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "DraggableMidiLabel.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

private:
    PluginProcessor& processor;
    juce::ComboBox genreSelector;
    juce::TextButton generateButton;
    DraggableMidiLabel dragArea;
    juce::LookAndFeel_V4 lookAndFeel;

    juce::String getGenreName(int index) const;
};