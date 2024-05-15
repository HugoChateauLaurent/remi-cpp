/*
  ==============================================================================

    ReservoirAudioFX.h
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include "Reservoir.h"
#include "SparseMatrixGenerator.h"


class ReservoirAudioFX {
public:
    ReservoirAudioFX();
    virtual ~ReservoirAudioFX() = default;
    ReservoirNetwork reservoir;
    std::vector<std::vector<float>> readout;
    std::vector<float> readout_return;
    std::vector<std::vector<float>> reservoir_state;    
    std::vector<float> reservoir_state_return;
    std::vector<float> output;                          //change to 1D vector
    std::vector<float> old_output;                      //change to 1D vector
    float feedback_mix = 0.0f;
    
    int seed;

    
    std::default_random_engine generator;        // Random number generator


    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
    std::vector<float> forward(int pattern);
    //void decode_state();
    float decode_state(int i, std::vector<float> reservoir_state);
    std::vector<float> get_state();
    std::vector<float> get_readout();


    void set_sr(float sr);
    void set_lr(float lr);
    void set_input_scaling(float input_scaling);
    void set_units(int units);
    void set_noise_rc(float noise_rc);
    
    float outputGain = 0.0f;

//private:
    
};