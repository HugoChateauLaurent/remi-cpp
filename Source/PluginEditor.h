/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Component/HorizontalMeter.h"
#include "Component/State.h"


//==============================================================================
/**
*/
class ReMiAudioProcessorEditor: public juce::AudioProcessorEditor, public Timer 
{
public:
    ReMiAudioProcessorEditor (ReMiAudioProcessor&);
    
    ~ReMiAudioProcessorEditor() override;
   
    //==============================================================================
    void timerCallback() override;
    void paint (juce::Graphics&) override;
    void resized() override;
    void reset();
    void ReMiAudioProcessorEditor::plot_state();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReMiAudioProcessor& audioProcessor;
    Gui::HorizontalMeter horizontalMeter;
    NeuronState::State state;
    juce::TextButton resetButton;
    bool show_state = false;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessorEditor)
};