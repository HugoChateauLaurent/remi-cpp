/*
  ==============================================================================

   This file is part of the JUCE examples.
   Copyright (c) 2022 - Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   To use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES,
   WHETHER EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR
   PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:                  ArpeggiatorPlugin
 version:               1.0.0
 vendor:                JUCE
 website:               http://juce.com
 description:           Arpeggiator audio plugin.

 dependencies:          juce_audio_basics, juce_audio_devices, juce_audio_formats,
                        juce_audio_plugin_client, juce_audio_processors,
                        juce_audio_utils, juce_core, juce_data_structures,
                        juce_events, juce_graphics, juce_gui_basics, juce_gui_extra
 exporters:             xcode_mac, vs2022

 moduleFlags:           JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:                  AudioProcessor
 mainClass:             Arpeggiator

 useLocalCopy:          1

 pluginCharacteristics: pluginWantsMidiIn, pluginProducesMidiOut, pluginIsMidiEffectPlugin

 END_JUCE_PIP_METADATA

*******************************************************************************/

#pragma once


//==============================================================================
class Arpeggiator final : public AudioProcessor
{
public:
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

    //==============================================================================
    Arpeggiator()
        : AudioProcessor (BusesProperties()) // add no audio buses at all
    {
        rate = new AudioParameterInt("rate", "Arpeggiator Rate", 0, NumDivisions - 1, 3); // Default to 1/4
        addParameter(rate);
    }

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override
    {
        ignoreUnused(samplesPerBlock);

        notes.clear();
        currentNote = 0;
        lastNoteValue = -1;
        time = 0;
        rateValue = static_cast<float>(sampleRate); // Initialize rateValue with sampleRate
    }

    void releaseResources() override {}

    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midi) override
    {
        // A pure MIDI plugin shouldn't be provided any audio data
        jassert(buffer.getNumChannels() == 0);

        // however we use the buffer to get timing information
        auto numSamples = buffer.getNumSamples();

        AudioPlayHead* playHead = getPlayHead();
        if (playHead == nullptr) return;

        AudioPlayHead::CurrentPositionInfo positionInfo;
        playHead->getCurrentPosition(positionInfo); // get the current position from the playhead

        auto bpm = positionInfo.bpm;                //bpm is quarterNotesPerMinute
        auto bps = bpm / 60;                        //bps is quarterNotesPerSecond
        auto samplesPerBeat = rateValue / bps;      //number of samples per beat/quarternote is samples per sec / beats per second

        // Calculate note duration based on the rate parameter
        auto division = static_cast<MusicalDivision>(rate->get());
        auto noteDuration = static_cast<int>(samplesPerBeat / std::pow(2.0, division));

        for (const auto metadata : midi)
        {
            const auto msg = metadata.getMessage();
            if (msg.isNoteOn())  notes.add(msg.getNoteNumber());
            else if (msg.isNoteOff()) notes.removeValue(msg.getNoteNumber());
        }

        midi.clear();
        time = (time + numSamples) % noteDuration;

        if ((time + numSamples) >= noteDuration)
        {
            auto offset = jmax(0, jmin((int)(noteDuration - time), numSamples - 1));

            if (lastNoteValue > 0)
            {
                midi.addEvent(MidiMessage::noteOff(1, lastNoteValue), offset);
                lastNoteValue = -1;
            }

            if (notes.size() > 0)
            {
                currentNote = (currentNote + 1) % notes.size();
                lastNoteValue = notes[currentNote];
                midi.addEvent(MidiMessage::noteOn(1, lastNoteValue, (uint8)127), offset);
            }

        }

    }

    using AudioProcessor::processBlock;

    //==============================================================================
    bool isMidiEffect() const override { return true; }

    //==============================================================================
    AudioProcessorEditor* createEditor() override { return new GenericAudioProcessorEditor(*this); }
    bool hasEditor() const override { return true; }

    //==============================================================================
    const String getName() const override { return "Arpeggiator"; }

    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return true; }
    double getTailLengthSeconds() const override { return 0; }

    //==============================================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const String getProgramName(int) override { return "None"; }
    void changeProgramName(int, const String&) override {}

    //==============================================================================
    void getStateInformation(MemoryBlock& destData) override
    {
        MemoryOutputStream(destData, true).writeFloat(rateValue);
    }

    void setStateInformation(const void* data, int sizeInBytes) override
    {
        rateValue = MemoryInputStream(data, static_cast<size_t>(sizeInBytes), false).readFloat();
    }

private:
    //==============================================================================
    AudioParameterInt* rate; // Updated parameter type
    int currentNote, lastNoteValue;
    int time;
    float rateValue; // Updated variable type
    SortedSet<int> notes;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Arpeggiator)
};