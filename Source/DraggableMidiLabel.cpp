#include "DraggableMidiLabel.h"

DraggableMidiLabel::DraggableMidiLabel()
{
    setOpaque(true);
}

void DraggableMidiLabel::setMidiFile(const juce::File& file, const juce::String& genre)
{
    midiFile = file;
    genreName = genre;
    state = State::Ready;
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
    repaint();
}

void DraggableMidiLabel::clear()
{
    midiFile = juce::File();
    genreName.clear();
    state = State::Empty;
    setMouseCursor(juce::MouseCursor::NormalCursor);
    repaint();
}

void DraggableMidiLabel::paint(juce::Graphics& g)
{
    auto bg = getBackgroundColour();
    g.fillAll(bg);

    // Borde sutil
    g.setColour(bg.brighter(isHovered ? 0.3f : 0.15f));
    g.drawRect(getLocalBounds(), isHovered ? 2 : 1);

    // Texto principal
    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(16.0f, juce::Font::bold));

    auto text = getDisplayText();
    g.drawFittedText(text, getLocalBounds().reduced(10), juce::Justification::centred, 2);

    // Subtexto con instruccion
    if (state == State::Ready || state == State::Dragging)
    {
        g.setFont(juce::Font(11.0f));
        g.setColour(juce::Colours::white.withAlpha(0.7f));
        g.drawFittedText("Arrastra al Channel Rack o Piano Roll",
                         getLocalBounds().reduced(10).translated(0, 18),
                         juce::Justification::centred, 1);
    }
}

void DraggableMidiLabel::mouseEnter(const juce::MouseEvent&)
{
    isHovered = true;
    repaint();
}

void DraggableMidiLabel::mouseExit(const juce::MouseEvent&)
{
    isHovered = false;
    repaint();
}

void DraggableMidiLabel::mouseDown(const juce::MouseEvent&)
{
    if (state == State::Ready && midiFile.existsAsFile())
    {
        state = State::Dragging;
        repaint();
    }
}

void DraggableMidiLabel::mouseDrag(const juce::MouseEvent& e)
{
    if (state == State::Dragging && midiFile.existsAsFile())
    {
        if (e.getDistanceFromDragStart() > 5)
        {
            juce::StringArray paths;
            paths.add(midiFile.getFullPathName());

            juce::DragAndDropContainer::performExternalDragDropOfFiles(paths, true, this);

            state = State::Ready;
            repaint();
        }
    }
}

juce::Colour DraggableMidiLabel::getBackgroundColour() const
{
    switch (state)
    {
        case State::Empty:
            return juce::Colour(0xFF2A2A2A); // gris oscuro
        case State::Ready:
            return isHovered ? juce::Colour(0xFF00CC44) : juce::Colour(0xFF00AA33); // verde
        case State::Dragging:
            return juce::Colour(0xFF008822); // verde oscuro
        default:
            return juce::Colours::darkgrey;
    }
}

juce::String DraggableMidiLabel::getDisplayText() const
{
    switch (state)
    {
        case State::Empty:
            return "Genera un beat primero";
        case State::Ready:
        case State::Dragging:
            return "LISTO: BEAT DE " + genreName.toUpperCase();
        default:
            return "";
    }
}
