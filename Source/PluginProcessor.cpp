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
ReMiAudioProcessor::ReMiAudioProcessor()
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
        addParameter (input_scaling_parameter = new juce::AudioParameterFloat ("input_scaling", // parameterID
                                                            "Input Scaling", // parameter name
                                                            0.0f,   // minimum value
                                                            5.0f,   // maximum value
                                                            1.0f)); // default value

        addParameter (min_volume_parameter = new juce::AudioParameterFloat ("min_volume", // parameterID
                                                            "Min Volume", // parameter name
                                                            0.0f,   // minimum value
                                                            1.0f,   // maximum value
                                                            0.0f)); // default value

        addParameter (max_volume_parameter = new juce::AudioParameterFloat ("max_volume", // parameterID
                                                            "Max Volume", // parameter name
                                                            0.0f,   // minimum value
                                                            1.0f,   // maximum value
                                                            1.0f)); // default value
                                                            
        addParameter (outputGain_parameter = new juce::AudioParameterFloat ("outputGain", // parameterID
                                                            "Output Gain", // parameter name
                                                            0.0f,   // minimum value
                                                            1000.0f,   // maximum value
                                                            1.0f)); // default value
                                                            
        addParameter (leak_rate_parameter = new juce::AudioParameterFloat ("leak_rate", // parameterID
                                                            "leak_rate_parameter", // parameter name
                                                            0.0f,   // minimum value
                                                            1.0f,   // maximum value
                                                            1.0f)); // default value
        addParameter (spectral_radius_parameter = new juce::AudioParameterFloat ("spectral_radius_parameter", // parameterID
                                                            "spectral_radius_parameter", // parameter name
                                                            0.0f,   // minimum value
                                                            10.0f,   // maximum value
                                                            1.0f)); // default value
        addParameter (feedback_mix_parameter = new juce::AudioParameterFloat ("feedback_mix_parameter", // parameterID
                                                            "feedback_mix_parameter", // parameter name
                                                            0.0f,   // minimum value
                                                            5.0f,   // maximum value
                                                            0.0f)); // default value
        addParameter (rate_parameter = new juce::AudioParameterInt ("rate", // parameterID
                                                            "rate", // parameter name
                                                            0,   // minimum value
                                                            NumDivisions - 1,   // maximum value
                                                            3)); // default value
        addParameter (pattern_parameter = new juce::AudioParameterInt ("pattern", // parameterID
                                                            "pattern", // parameter name
                                                            1,   // minimum value
                                                            3,   // maximum value
                                                            1)); // default value
        addParameter (neuron_numbers = new juce::AudioParameterInt ("neuron_numbers", // parameterID
                                                            "neuron_numbers", // parameter name
                                                            1,   // minimum value
                                                            64,   // maximum value
                                                            16)); // default value
        addParameter (display_time = new juce::AudioParameterInt ("display_time", // parameterID
                                                            "display_time", // parameter name
                                                            1,   // minimum value
                                                            10,   // maximum value
                                                            5)); // default value
                                                            
                                                            
    }
    
    
    
}

ReMiAudioProcessor::~ReMiAudioProcessor()
{
}

//==============================================================================
const juce::String ReMiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ReMiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ReMiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ReMiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ReMiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ReMiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ReMiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ReMiAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ReMiAudioProcessor::getProgramName (int index)
{
    return {};
}

void ReMiAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ReMiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // juce::dsp::ProcessSpec spec;
    // spec.sampleRate = sampleRate;
    // spec.maximumBlockSize = samplesPerBlock;
    // spec.numChannels = getTotalNumOutputChannels();

    // highPassFilter.reset();
    // *highPassFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 2000.0f);//5.0f);
    // highPassFilter.prepare(spec);

    currentVolume = 0.0f;
    time = 0;
    rateValue = static_cast<float>(sampleRate); // Initialize rateValue with sampleRate
 

    

}

void ReMiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ReMiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ReMiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    reservoirFX.reservoir.units=*neuron_numbers;
    
    auto numSamples = buffer.getNumSamples();

    juce::AudioPlayHead* playHead = getPlayHead();
    if (playHead == nullptr) return;

    AudioPlayHead::CurrentPositionInfo info;
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    playHead->getCurrentPosition(positionInfo); // get the current position from the playhead

    auto bpm = positionInfo.bpm;                //bpm is quarterNotesPerMinute
    auto bps = bpm / 60;                        //bps is quarterNotesPerSecond
    auto samplesPerBeat = rateValue / bps;      //number of samples per beat/quarternote is samples per sec / beats per second

    // Calculate note duration based on the rate parameter
    auto division = static_cast<MusicalDivision>(rate_parameter->get());
    auto intervalDuration = static_cast<int>(samplesPerBeat / std::pow(2.0, division));

    time = (time + numSamples) % intervalDuration;
    
    if ((time + numSamples) >= intervalDuration)
    {
        // currentVolume = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        currentVolume = reservoirFX.forward((pattern_parameter->get()) - 1);
        currentVolume = (currentVolume - min_volume_parameter->get()) / (max_volume_parameter->get() - min_volume_parameter->get());
        currentVolume = juce::jlimit(0.0f, 1.0f, currentVolume); // Ensure currentVolume is between 0 and 1
     
    }



    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    reservoirFX.reservoir.input_scaling=*input_scaling_parameter;
    reservoirFX.reservoir.lr=*leak_rate_parameter;
    reservoirFX.outputGain=*outputGain_parameter;
    reservoirFX.feedback_mix=*feedback_mix_parameter;
    reservoirFX.reservoir.sr=*spectral_radius_parameter;
    
    
    for (auto i = 0; i < totalNumOutputChannels; ++i)
    {
        auto* channelData = buffer.getWritePointer(i);
        
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            channelData[sample] = channelData[sample] * currentVolume; 
        }
    }

    std::ofstream logFile ("E:\\U-Bordeaux\\hackrob\\Remi\\log.csv", std::ios_base::app);
    logFile << currentVolume << "\n";
    logFile.close();

}

//==============================================================================
bool ReMiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ReMiAudioProcessor::createEditor()
{
    return new ReMiAudioProcessorEditor (*this);
}

//==============================================================================

float ReMiAudioProcessor::getModulationValue() const
{
    return currentVolume;
}


void ReMiAudioProcessor::addArrayToXml(XmlElement* xml, const String& arrayName, const std::vector<std::vector<float>>& array)
{
    XmlElement* arrayElement = xml->createNewChildElement(arrayName);
    for (const auto& row : array)
    {
        XmlElement* rowElement = arrayElement->createNewChildElement("Row");
        for (float value : row)
        {
            rowElement->createNewChildElement("Value")->setAttribute("val", value);
        }
    }
}

void ReMiAudioProcessor::addArrayToXml(XmlElement* xml, const String& arrayName, const std::vector<float>& array)
{
    XmlElement* arrayElement = xml->createNewChildElement(arrayName);
    for (float value : array)
    {
        arrayElement->createNewChildElement("Value")->setAttribute("val", value);
    }
}

void ReMiAudioProcessor::loadArrayFromXml(XmlElement* xml, const String& arrayName, std::vector<std::vector<float>>& array)
{
    std::vector<std::vector<float>> array_temp;
    
    if (auto* arrayElement = xml->getChildByName(arrayName))
    {
        //array.clear();
        for (auto* rowElement : arrayElement->getChildIterator())
        {
            std::vector<float> row;
            for (auto* valueElement : rowElement->getChildIterator())
            {
                row.push_back(valueElement->getDoubleAttribute("val"));
            }
            array_temp.push_back(row);
        }
    }
    array = array_temp;
}

void ReMiAudioProcessor::loadArrayFromXml(XmlElement* xml, const String& arrayName, std::vector<float>& array)
{
    std::vector<float>array_temp;
    
    if (auto* arrayElement = xml->getChildByName(arrayName))
    {
        
        //array.clear();
        for (auto* valueElement : arrayElement->getChildIterator())
        {
            array_temp.push_back(valueElement->getDoubleAttribute("val"));
        }
    }
    array = array_temp;
}

void ReMiAudioProcessor::getStateInformation (MemoryBlock& destData) 
{
    // Create an XML element for the state
    std::unique_ptr<XmlElement> xml (new XmlElement("state"));

    // Add parameter state
    xml->setAttribute("input_scaling_parameter",*input_scaling_parameter);
    xml->setAttribute("feedback_mix_parameter", *feedback_mix_parameter);
    xml->setAttribute("outputGain_parameter",*outputGain_parameter);
    xml->setAttribute("min_volume_parameter", *min_volume_parameter);
    xml->setAttribute("max_volume_parameter", *max_volume_parameter);
    xml->setAttribute("leak_rate_parameter", *leak_rate_parameter);
    xml->setAttribute("spectral_radius_parameter",*spectral_radius_parameter);
    xml->setAttribute("rate_parameter", *rate_parameter);
    xml->setAttribute("pattern_parameter", *pattern_parameter);
    xml->setAttribute("neuron_numbers", *neuron_numbers);
    xml->setAttribute("Old_output", reservoirFX.old_output);

    // Add custom data to XML
    addArrayToXml(xml.get(), "W", reservoirFX.reservoir.W);
    addArrayToXml(xml.get(), "Win", reservoirFX.reservoir.Win);
    addArrayToXml(xml.get(), "State", reservoirFX.reservoir_state);
    addArrayToXml(xml.get(), "Readout", reservoirFX.get_readout());
    addArrayToXml(xml.get(), "Bias", reservoirFX.reservoir.bias);

    // Convert XML to binary
    copyXmlToBinary(*xml, destData);
}

void ReMiAudioProcessor::setStateInformation (const void* data, int sizeInBytes) 
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState != nullptr)
    {
        // Check if the XML contains our parameters state
        if (xmlState->hasTagName("state"))
        {
            
            *input_scaling_parameter = xmlState->getDoubleAttribute("input_scaling_parameter");
            *feedback_mix_parameter = xmlState->getDoubleAttribute("feedback_mix_parameter");
            *outputGain_parameter = xmlState->getDoubleAttribute("outputGain_parameter");
            *min_volume_parameter = xmlState->getDoubleAttribute("min_volume_parameter");
            *max_volume_parameter = xmlState->getDoubleAttribute("max_volume_parameter");
            *leak_rate_parameter = xmlState->getDoubleAttribute("leak_rate_parameter");
            *spectral_radius_parameter = xmlState->getDoubleAttribute("spectral_radius_parameter");
            *rate_parameter = xmlState->getDoubleAttribute("rate_parameter");
            *pattern_parameter = xmlState->getDoubleAttribute("pattern_parameter");
            *neuron_numbers = xmlState->getDoubleAttribute("neuron_numbers");
            reservoirFX.old_output  = xmlState->getDoubleAttribute("Old_output");

            // Load custom data from XML
            loadArrayFromXml(xmlState.get(), "W", reservoirFX.reservoir.W);
            loadArrayFromXml(xmlState.get(), "Win", reservoirFX.reservoir.Win);
            loadArrayFromXml(xmlState.get(), "State", reservoirFX.reservoir_state);
            loadArrayFromXml(xmlState.get(), "Readout", reservoirFX.get_readout());
            loadArrayFromXml(xmlState.get(), "Bias", reservoirFX.reservoir.bias);

        }
    }
}






//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ReMiAudioProcessor();
}
