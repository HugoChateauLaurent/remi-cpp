/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <memory>
#include "PluginProcessor.h"
#include "Component/HorizontalMeter.h"
#include "Component/State.h"
#include "Component/ColorGridComponent.h"
#include "Component/ColorNet.h"
#include "Component/FeatureGenerator.h"

//==============================================================================
/**
*/
class ReMiAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer 
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
    void generateAndApplyColors();

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
    std::unique_ptr<juce::FileChooser> myChooser;
    std::unique_ptr<ColorGridComponent> colorGrid;

    int rows;
    int cols;
    int num_random_vars = 12;
    std::unique_ptr<ColorNet> color_net;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessorEditor)
};
