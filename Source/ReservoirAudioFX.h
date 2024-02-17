/*
  ==============================================================================

    ReservoirAudioFX.h
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include "Reservoir.h"


class ReservoirAudioFX {
public:
    ReservoirAudioFX();
    virtual ~ReservoirAudioFX() = default;
    Reservoir reservoir;
    Eigen::SparseMatrix<float> readout;
    Eigen::VectorXf reservoir_state;
    float output = 0.0f;
    float readout_connectivity = 0.1f;
    
    unsigned seed;

    
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
    
    float outputGain = 0.0f;

//private:
    
};