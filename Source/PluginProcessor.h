/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ReservoirArp.h"

//==============================================================================
/**
*/
class ReMiArpProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ReMiArpProcessor();
    ~ReMiArpProcessor() override;

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

    ReservoirArp reservoirArp;
        
    juce::AudioParameterFloat* feedback_parameter;
    juce::AudioParameterFloat* leak_rate_parameter;
    juce::AudioParameterFloat* spectral_radius_parameter;
    juce::AudioParameterFloat* softmax_beta_parameter;
    juce::AudioParameterChoice* rate_parameter;
    juce::AudioParameterInt* neuron_numbers;
    juce::AudioParameterInt* display_time;

private:
    //==============================================================================
    
    // Arpeggiator state
    int currentNoteValue;
    float currentSampleRate;
    float currentBPM;
    juce::SortedSet<int> notes;
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiArpProcessor)

};