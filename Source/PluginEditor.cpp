/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Component/FeatureGenerator.h"
#include "Component/ColorNet.h"
#include "Component/ActivationFunctions.h"

// This function can be used to convert parameters of the UI into values in the correct range
float convertToNewRange(float oldValue, float oldMin, float oldMax, float newMin, float newMax) {
    float newValue = (((oldValue - oldMin) * (newMax - newMin)) / (oldMax - oldMin)) + newMin;
    return newValue;
}

//==============================================================================
ReMiAudioProcessorEditor::ReMiAudioProcessorEditor (ReMiAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    addAndMakeVisible(horizontalMeter); 
    addAndMakeVisible(state);
    addAndMakeVisible(resetButton);
    addAndMakeVisible(reset_parametersButton);
    addAndMakeVisible(saveButton);
    addAndMakeVisible(loadButton);

    // Show state plot button
    resetButton.setButtonText ("Plot");
    resetButton.setCentrePosition(750,250);
    resetButton.setSize (40, 30);
    resetButton.onClick = [this] { plot_state(); }; 

    // Reset parameters button
    reset_parametersButton.setButtonText ("Reset");
    reset_parametersButton.setCentrePosition(600,250);
    reset_parametersButton.setSize (40, 30);
    reset_parametersButton.onClick = [this] { reset_parameters(); }; 

    // Save button
    saveButton.setButtonText ("Save");
    saveButton.setCentrePosition(700,250);
    saveButton.setSize (40, 30);
    saveButton.onClick = [this] { saveParameters(); }; 

    // Load button
    loadButton.setButtonText ("Load");
    loadButton.setCentrePosition(650,250);
    loadButton.setSize (40, 30);
    loadButton.onClick = [this] { loadParameters(); }; 

    // Grid dimensions
    rows = 100;
    cols = 100;

    // Initialize color grid with black colors
    std::vector<juce::Colour> colors(rows * cols, juce::Colours::black);

    colorGrid = std::make_unique<ColorGridComponent>(colors, rows, cols);
    addAndMakeVisible(colorGrid.get());

    // Initialize color_net using std::make_unique
    color_net = std::make_unique<ColorNet>(
        3 + num_random_vars,
        std::vector<int>{10, 10, 10, 10, 10},
        std::vector<std::string>{"sinusoid", "sinusoid", "sinusoid", "sinusoid", "sinusoid"}
    );

    setSize(512, 512);
    startTimerHz(horizontalMeter.HZ);
}

ReMiAudioProcessorEditor::~ReMiAudioProcessorEditor()
{
    stopTimer();
}

void ReMiAudioProcessorEditor::timerCallback()
{
    horizontalMeter.maxDataPoints =  *audioProcessor.display_time * horizontalMeter.HZ;

    horizontalMeter.setLevel(audioProcessor.getModulationValue());
    horizontalMeter.repaint();
    state.get_neuron_numbers(audioProcessor.reservoirFX.reservoir.units);

    state.setOpaque(true);
    state.setRepaintsOnMouseActivity(false);  
    state.setBufferedToImage(true);
    horizontalMeter.setEnabled(false);
    state.setVisible(false);
    state.setEnabled(false);
    state.state = audioProcessor.reservoirFX.reservoir_state;
    state.setColor(audioProcessor.reservoirFX.get_readout());

    state.repaint();

    if (state.neuron_change == true)
    {
        state.set = 0;
        state.numLines = audioProcessor.reservoirFX.reservoir.units;
        reset();
    }
    else if (show_state == true)
    {
        state.setOpaque(false);
        state.setVisible(true);
        horizontalMeter.setEnabled(true);
        state.setEnabled(true);
        state.state = audioProcessor.reservoirFX.reservoir_state;
        state.setColor(audioProcessor.reservoirFX.get_readout());
        state.repaint();
    }

    // Generate or fetch new color data
    generateAndApplyColors();
}

void ReMiAudioProcessorEditor::generateAndApplyColors()
{
    std::vector<juce::Colour> colors;
    colors.reserve(rows * cols);

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            // Generate input features for the neural network
            std::vector<double> features = generateInputFeatures(x, y, cols, rows, audioProcessor.audioVars);

            // Initialize input_features
            std::vector<std::vector<double>> input_features = { features };

            // Forward pass through the neural network
            auto output = color_net->forward(input_features);

            // Convert output to RGB values (0-255)
            uint8_t red = static_cast<uint8_t>(output[0][0] * 255);
            uint8_t green = static_cast<uint8_t>(output[0][1] * 255);
            uint8_t blue = static_cast<uint8_t>(output[0][2] * 255);

            colors.emplace_back(juce::Colour::fromRGB(red, green, blue));
        }
    }

    // Update the color grid component
    colorGrid->updateColors(colors);
}

//==============================================================================

void ReMiAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Optional) Add custom painting code here
}

void ReMiAudioProcessorEditor::resized()
{
    horizontalMeter.setBounds(getLocalBounds());
    state.setBounds(getLocalBounds());
    colorGrid->setBounds(getLocalBounds());
}

void ReMiAudioProcessorEditor::reset_parameters()
{
    *audioProcessor.input_scaling_parameter = 1.0;
    *audioProcessor.feedback_mix_parameter = 0.0;
    *audioProcessor.outputGain_parameter = 1.0;
    *audioProcessor.min_volume_parameter = 0.0;
    *audioProcessor.max_volume_parameter = 1.0;
    *audioProcessor.leak_rate_parameter = 1.0;
    *audioProcessor.spectral_radius_parameter = 1.0;
    *audioProcessor.rate_parameter = 3;
    *audioProcessor.pattern_parameter = 1;

    audioProcessor.reservoirFX.initialize(true); 
    audioProcessor.reservoirFX.reservoir.initialize(true);
}

void ReMiAudioProcessorEditor::reset()
{
    audioProcessor.reservoirFX.initialize(true); 
    audioProcessor.reservoirFX.reservoir.initialize(true);

    // Reset 2D queue in state plot
    state.state.clear();
    state.lines.clear();

    for (int i = 0; i < audioProcessor.reservoirFX.reservoir.units; i++)
    {
        std::deque<float> line(151, 0.0f);
        state.lines.push_back(line);
    }

    state.setState(audioProcessor.reservoirFX.get_state());
    state.setColor(audioProcessor.reservoirFX.get_readout());
}

void ReMiAudioProcessorEditor::plot_state()
{
    // Toggle state plot visibility
    show_state = !show_state;
}

void ReMiAudioProcessorEditor::saveParameters()
{
    myChooser = std::make_unique<juce::FileChooser>("Save Parameters",
                                                    juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                    "*.params");

    auto folderChooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;

    myChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
    {
        juce::File file(chooser.getResult());

        if (file != juce::File{})  // Check if a valid file is chosen
        {
            juce::MemoryBlock destData;
            audioProcessor.getStateInformation(destData);

            file.replaceWithData(destData.getData(), destData.getSize());
        }
    });
}

void ReMiAudioProcessorEditor::loadParameters()
{
    show_state = false;
    state.setOpaque(true);
    state.setRepaintsOnMouseActivity(false);  
    state.setBufferedToImage(true);
    horizontalMeter.setEnabled(false);
    state.setVisible(false);
    state.setEnabled(false);

    *audioProcessor.input_scaling_parameter = 1.0;
    *audioProcessor.feedback_mix_parameter = 0.0;
    *audioProcessor.outputGain_parameter = 1.0;
    *audioProcessor.min_volume_parameter = 0.0;
    *audioProcessor.max_volume_parameter = 1.0;
    *audioProcessor.leak_rate_parameter = 1.0;
    *audioProcessor.spectral_radius_parameter = 1.0;
    *audioProcessor.rate_parameter = 3;
    *audioProcessor.pattern_parameter = 1;

    myChooser = std::make_unique<juce::FileChooser>("Load Parameters",
                                                    juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
                                                    "*.params");

    auto folderChooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

    myChooser->launchAsync(folderChooserFlags, [this](const juce::FileChooser& chooser)
    {
        juce::File file(chooser.getResult());

        if (file != juce::File{})  // Check if a valid file is chosen
        {
            juce::MemoryBlock data;
            if (file.loadFileAsData(data))
            {
                audioProcessor.setStateInformation(data.getData(), static_cast<int>(data.getSize()));
            }
        }
    });
}
