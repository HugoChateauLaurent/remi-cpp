/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/HorizontalMeter.h"

//==============================================================================
/**
*/
class ReMiAudioProcessorEditor  : public juce::AudioProcessorEditor, public Timer
{
public:
    ReMiAudioProcessorEditor (ReMiAudioProcessor&);
    ~ReMiAudioProcessorEditor() override;

    //==============================================================================
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReMiAudioProcessor& audioProcessor;
    Gui::HorizontalMeter horizontalMeter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessorEditor)
};