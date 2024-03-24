/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ReservoirAudioFX.h"

//==============================================================================
/**
*/
class ReMiAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ReMiAudioProcessor();
    ~ReMiAudioProcessor() override;

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

    ReservoirAudioFX reservoirFX;
    
    float getModulationValue() const;

private:
    //==============================================================================
    juce::AudioParameterFloat* input_scaling_parameter;
    juce::AudioParameterFloat* feedback_mix_parameter;
    juce::AudioParameterFloat* outputGain_parameter;
    juce::AudioParameterFloat* min_volume_parameter;
    juce::AudioParameterFloat* max_volume_parameter;
    juce::AudioParameterFloat* leak_rate_parameter;
    juce::AudioParameterFloat* spectral_radius_parameter;
    juce::AudioParameterInt* rate_parameter;
    juce::AudioParameterInt* pattern_parameter;
    // Enumeration for musical divisions
    enum MusicalDivision
    {
        Div2 = 0,
        Div1,
        DivHalf,
        DivQuarter,
        DivEighth,
        DivSixteenth,
        DivThirtySecond,
        DivSixtyFourth,
        Div128th,
        Div256th,
        Div512th,
        Div1024th,
        Div2048th,
        Div4096th,
        Div8192th,
        Div16384th,
        Div32768th,
        Div65536th,
        NumDivisions
    };
    float currentVolume;
    int time;
    float rateValue; // Updated variable type

    // juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessor)

};