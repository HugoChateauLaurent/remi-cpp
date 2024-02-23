/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

// This function can be used to convert parameters of the UI into values in the correct range
float convertToNewRange(float oldValue, float oldMin, float oldMax, float newMin, float newMax) {
    float newValue((((oldValue - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin);
    return newValue;
}


//==============================================================================
ReMiAudioProcessorEditor::ReMiAudioProcessorEditor (ReMiAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

ReMiAudioProcessorEditor::~ReMiAudioProcessorEditor()
{
}

//==============================================================================
void ReMiAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("ReMi HP", getLocalBounds(), juce::Justification::centred, 1);
}

void ReMiAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}