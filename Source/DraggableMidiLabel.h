#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class DraggableMidiLabel : public juce::Component
{
public:
    DraggableMidiLabel();

    void setMidiFile(const juce::File& file, const juce::String& genreName);
    void clear();

    void paint(juce::Graphics& g) override;
    void mouseEnter(const juce::MouseEvent&) override;
    void mouseExit(const juce::MouseEvent&) override;
    void mouseDown(const juce::MouseEvent&) override;
    void mouseDrag(const juce::MouseEvent&) override;

private:
    enum class State
    {
        Empty,
        Ready,
        Dragging
    };

    State state = State::Empty;
    juce::File midiFile;
    juce::String genreName;
    bool isHovered = false;

    juce::Colour getBackgroundColour() const;
    juce::String getDisplayText() const;
};
