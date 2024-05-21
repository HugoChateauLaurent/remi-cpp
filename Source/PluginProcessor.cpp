/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Reservoir.h"
#include <random>
#include <iostream>
#include <fstream> 



//==============================================================================
ReMiArpProcessor::ReMiArpProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    {
        addParameter (leak_rate_parameter = new juce::AudioParameterFloat ("leak_rate", // parameterID
                                                            "Inertia", // parameter name
                                                            0.0f,   // minimum value
                                                            1.0f,   // maximum value
                                                            1.0f)); // default value
        addParameter (spectral_radius_parameter = new juce::AudioParameterFloat ("spectral_radius_parameter", // parameterID
                                                            "Chaos", // parameter name
                                                            0.0f,   // minimum value
                                                            10.0f,   // maximum value
                                                            1.0f)); // default value
        addParameter (softmax_beta_parameter = new juce::AudioParameterFloat ("softmax_beta_parameter", // parameterID
                                                            "Confidence", // parameter name
                                                            0.001f,   // minimum value
                                                            1000.0f,   // maximum value
                                                            1000.0f)); // default value
        addParameter (feedback_parameter = new juce::AudioParameterFloat ("feedback_parameter", // parameterID
                                                            "Feedback", // parameter name
                                                            0.0f,   // minimum value
                                                            100.0f,   // maximum value
                                                            0.0f)); // default value
        addParameter (rate_parameter = new juce::AudioParameterChoice ("rate", // parameterID
                                                            "Rate", // parameter name
                                                            {"1/128", "1/96", "1/64", "1/48", "1/32", "1/24", "1/16", "1/12", "1/8", "1/6", "1/4", "1/3", "1/2", "1/1"}, // possible choices
                                                            8)); // default value (1/8)
        addParameter (neuron_numbers = new juce::AudioParameterInt ("neuron_numbers", // parameterID
                                                            "Neurons", // parameter name
                                                            1,   // minimum value
                                                            300,   // maximum value
                                                            16)); // default value
        addParameter (display_time = new juce::AudioParameterInt ("display_time", // parameterID
                                                            "Display Time", // parameter name
                                                            1,   // minimum value
                                                            10,   // maximum value
                                                            5)); // default value
                                                            
                                                            
    }
    
    
    
}

ReMiArpProcessor::~ReMiArpProcessor()
{
}

//==============================================================================
const juce::String ReMiArpProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReMiArpProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReMiArpProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReMiArpProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReMiArpProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReMiArpProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReMiArpProcessor::getCurrentProgram()
{
    return 0;
}

void ReMiArpProcessor::setCurrentProgram (int index)
{
}

const juce::String ReMiArpProcessor::getProgramName (int index)
{
    return {};
}

void ReMiArpProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReMiArpProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    notes.clear();
    currentNoteValue = -1;
    currentSampleRate = static_cast<float> (sampleRate);
}

void ReMiArpProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReMiArpProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ReMiArpProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{

    reservoirArp.reservoir.units=*neuron_numbers;

    juce::ScopedNoDenormals noDenormals;
    reservoirArp.reservoir.lr=*leak_rate_parameter;
    reservoirArp.reservoir.sr=*spectral_radius_parameter;

    // Retrieve the current position info from the host
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    if (auto* playHead = getPlayHead())
    {
        playHead->getCurrentPosition(positionInfo);
        currentBPM = positionInfo.bpm; // Update the BPM from the host
    }

    // Retrieve the rate from the parameter and map to note duration
    float rate = 1.0f; // Default to Quarter Note
    switch (rate_parameter->getIndex())
    {
        case 0: rate = 1.0f / 128.0f; break; // 1/128 note
        case 1: rate = 1.0f / 96.0f; break;  // 1/96 note
        case 2: rate = 1.0f / 64.0f; break;  // 1/64 note
        case 3: rate = 1.0f / 48.0f; break;  // 1/48 note
        case 4: rate = 1.0f / 32.0f; break;  // 1/32 note
        case 5: rate = 1.0f / 24.0f; break;  // 1/24 note
        case 6: rate = 1.0f / 16.0f; break;  // 1/16 note
        case 7: rate = 1.0f / 12.0f; break;  // 1/12 note
        case 8: rate = 1.0f / 8.0f; break;   // 1/8 note
        case 9: rate = 1.0f / 6.0f; break;   // 1/6 note
        case 10: rate = 1.0f / 4.0f; break;  // 1/4 note
        case 11: rate = 1.0f / 3.0f; break;  // 1/3 note
        case 12: rate = 1.0f / 2.0f; break;  // 1/2 note
        case 13: rate = 1.0f; break;        // 1/1 note (whole note)
        default: rate = 1.0f / 8.0f; break; // Default to 1/8 note
    }
    
    auto numSamples = buffer.getNumSamples();
    
    // Calculate the duration of one beat in seconds
    double beatDurationInSeconds = 60.0 / currentBPM;

    // Calculate the note duration in samples
    auto noteDuration = static_cast<int> (std::ceil (currentSampleRate * beatDurationInSeconds * rate));
    
    for (const auto metadata : midiMessages)
    {
        const auto msg = metadata.getMessage();
        if (msg.isNoteOn())  notes.add (msg.getNoteNumber());
        else if (msg.isNoteOff()) notes.removeValue (msg.getNoteNumber());
    }
    midiMessages.clear();

    // Calculate the current sample position within the current note duration
    int currentSamplePosition = positionInfo.timeInSamples % noteDuration;

    if ((currentSamplePosition + numSamples) >= noteDuration)
    {
        auto offset = juce::jmax (0, juce::jmin ((int) (noteDuration - currentSamplePosition), numSamples - 1));

        if (currentNoteValue >= 0)
        {
            midiMessages.addEvent (juce::MidiMessage::noteOff (1, currentNoteValue), offset);
            currentNoteValue = -1;
        }
        if (notes.size() > 0)
        {
            currentNoteValue = notes[reservoirArp.forward(notes.size(), *softmax_beta_parameter, *feedback_parameter)];
            midiMessages.addEvent(juce::MidiMessage::noteOn(1, currentNoteValue, (juce::uint8) 127), offset); // TODO: control velocity
        }
    }
    
    

}

//==============================================================================
bool ReMiArpProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReMiArpProcessor::createEditor()
{
    return new ReMiArpProcessorEditor (*this);
}

//==============================================================================
void ReMiArpProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

}

void ReMiArpProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.


}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReMiArpProcessor();
}
