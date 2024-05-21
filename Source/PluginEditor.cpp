/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


// This function can be used to convert parameters of the UI into values in the correct range
float convertToNewRange(float oldValue, float oldMin, float oldMax, float newMin, float newMax) {
    float newValue((((oldValue - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin);
    return newValue;
}


//==============================================================================
ReMiArpProcessorEditor::ReMiArpProcessorEditor (ReMiArpProcessor& p)
    : AudioProcessorEditor (&p), arpProcessor (p)
{
    
    addAndMakeVisible(horizontalMeter); 
    addAndMakeVisible(state);
    addAndMakeVisible(resetButton);

    //show state plot button
    resetButton.setButtonText ("Plot");
    resetButton.setCentrePosition(750,250);
    resetButton.setSize (40, 30);
    resetButton.onClick = [this] {plot_state();}; 

    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 300);
    
    startTimerHz(horizontalMeter.HZ);
}

ReMiArpProcessorEditor::~ReMiArpProcessorEditor()
{
    stopTimer();
}

void ReMiArpProcessorEditor::timerCallback()
{
    horizontalMeter.maxDataPoints =  *arpProcessor.display_time*horizontalMeter.HZ;
    state.maxDataPoints =  *arpProcessor.display_time*horizontalMeter.HZ;
    
    horizontalMeter.setLevel(0); // TODO: set this to the correct value
    horizontalMeter.repaint();
    state.get_neuron_numbers(arpProcessor.reservoirArp.reservoir.units);
    
    state.setOpaque(true);
    state.setRepaintsOnMouseActivity(false);  
    state.setBufferedToImage(true);
    horizontalMeter.setEnabled(false);
    state.setVisible(false);
    state.setEnabled(false);
    state.state = arpProcessor.reservoirArp.reservoir_state;
    // state.setColor(arpProcessor.reservoirArp.get_readout());

    state.repaint();
    

    if (state.neuron_change == true)
    {
        
        
        state.set = 0;
        state.numLines = arpProcessor.reservoirArp.reservoir.units;
        reset();

    }
    
    else if(show_state == true)
    {
            
        state.setOpaque(false);
        state.setVisible(true);
        horizontalMeter.setEnabled(true);
        state.setEnabled(true);
        state.state = arpProcessor.reservoirArp.reservoir_state;
        // state.setColor(arpProcessor.reservoirArp.get_readout());
        state.repaint();


    }
    
    
}

//==============================================================================
void ReMiArpProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("ReMi LFO", getLocalBounds(), juce::Justification::centred, 1);
    
    
}

void ReMiArpProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    horizontalMeter.setBounds(getLocalBounds());
    state.setBounds(getLocalBounds());

}

void ReMiArpProcessorEditor::reset()
{
        
        *arpProcessor.feedback_parameter = 0.0f;
        *arpProcessor.leak_rate_parameter = 1.0f;
        *arpProcessor.spectral_radius_parameter = 1.0f;
        *arpProcessor.softmax_beta_parameter = 1.0f;
        *arpProcessor.rate_parameter = 8;
        
        arpProcessor.reservoirArp.initialize(true); 
        arpProcessor.reservoirArp.reservoir.initialize(true);
        
        //reset 2D queue in state plot
        state.state.clear();
        state.lines.clear();
	            
	            
        for (int i = 0; i < arpProcessor.reservoirArp.reservoir.units; i++ )
        {
            std::deque<float> line;
            for (int j = 0; j < 151 ; j++)
            {
                line.push_back(0.0f);
            }
                
            state.lines.push_back(line);            
        }
        
        state.setState(arpProcessor.reservoirArp.get_state());
        // state.setColor(arpProcessor.reservoirArp.get_readout());

}

void ReMiArpProcessorEditor::plot_state()
{
        //show state plot or not

        if(show_state == false)
        {
            show_state = true;
        }
        else
        {
            show_state = false;
        }
}


    
