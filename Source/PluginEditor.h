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
#include "Component/ColorGridComponent.h"


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
    void plot_state();
    void reset_parameters();
    void saveParameters();
    void loadParameters();
    std::vector<juce::Colour> generateNewColorData();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ReMiAudioProcessor& audioProcessor;
    Gui::HorizontalMeter horizontalMeter;
    NeuronState::State state;
    juce::TextButton resetButton;
    juce::TextButton reset_parametersButton;
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    bool show_state = false;
    std::unique_ptr<FileChooser> myChooser;
    std::unique_ptr<ColorGridComponent> colorGrid;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessorEditor)
};