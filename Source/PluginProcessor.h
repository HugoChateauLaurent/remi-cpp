/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ReservoirAudioFX.h"
#include <mutex>

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
    
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


    //=============================================================================
    
    void addArrayToXml(XmlElement* xml, const String& arrayName, const std::vector<std::vector<float>>& array);
    void addArrayToXml(XmlElement* xml, const String& arrayName, const std::vector<float>& array);
    void loadArrayFromXml(XmlElement* xml, const String& arrayName, std::vector<std::vector<float>>& array);
    void loadArrayFromXml(XmlElement* xml, const String& arrayName, std::vector<float>& array);
    
    
    ReservoirAudioFX reservoirFX;
    
    float getModulationValue() const;
    
    juce::AudioParameterFloat* input_scaling_parameter;
    juce::AudioParameterFloat* feedback_mix_parameter;
    juce::AudioParameterFloat* outputGain_parameter;
    juce::AudioParameterFloat* min_volume_parameter;
    juce::AudioParameterFloat* max_volume_parameter;
    juce::AudioParameterFloat* leak_rate_parameter;
    juce::AudioParameterFloat* spectral_radius_parameter;
    juce::AudioParameterInt* rate_parameter;
    juce::AudioParameterInt* pattern_parameter;
    juce::AudioParameterInt* neuron_numbers;
    juce::AudioParameterInt* display_time;
    juce::AudioParameterFloat* random1_parameter;
    juce::AudioParameterFloat* random2_parameter;

    std::vector<double> randomVars = { 0.0, 0.0 };
    std::vector<double> audioVars = { 0.0, 0.0 };
    
    float currentVolume;
    int count = 0;

private:
    //==============================================================================
    
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
        NumDivisions
    };
    int time;
    float rateValue; // Updated variable type

    // juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> highPassFilter;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReMiAudioProcessor)

};