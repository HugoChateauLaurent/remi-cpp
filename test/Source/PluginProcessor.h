/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Reservoir.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    NewProjectAudioProcessor();
    ~NewProjectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    Reservoir Reservoir;
    
    addParameter (sr = new AudioParameterFloat ({ "sr", 1 }, "Chaos", 0.0, 10.0, 0.0));
    addParameter (lr = new AudioParameterFloat ({ "lr", 1 }, "Inertia", 0.0, 10.0, 0.0));
    addParameter (input_scaling = new AudioParameterFloat ({ "input_scaling", 1 }, "Feedback", 0.0, 10.0, 0.0));
    addParameter (units = new AudioParameterFloat ({ "units", 1 }, "Neurons", 0.0, 10.0, 0.0));
    addParameter (noise_rc = new AudioParameterFloat ({ "noise_rc", 1 }, "Noise", 0.0, 10.0, 0.0));
    addParameter (softmax_gain = new AudioParameterFloat ({ "softmax_gain", 1 }, "Confidence", 0.0, 10.0, 0.0));


private:
    //==============================================================================
    AudioParameterFloat* sr;
    AudioParameterFloat* lr;
    AudioParameterFloat* input_scaling;
    AudioParameterFloat* units;
    AudioParameterFloat* noise_rc;
    AudioParameterFloat* softmax_gain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessor)
};
