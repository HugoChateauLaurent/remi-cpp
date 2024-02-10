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
    Reservoir reservoir;
    std::vector<std::vector<float>> readout;
    std::vector<float> reservoir_state;
    std::vector<float> output;
    
    int seed;

    
    std::default_random_engine generator;        // Random number generator


    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
    float forward(float sample);
    void decode_state();


    void set_sr(float sr);
    void set_lr(float lr);
    void set_input_scaling(float input_scaling);
    void set_units(int units);
    void set_noise_rc(float noise_rc);

//private:
    
};