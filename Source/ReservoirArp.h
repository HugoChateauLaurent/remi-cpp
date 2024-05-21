/*
  ==============================================================================

    ReservoirArp.h
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include "Reservoir.h"
#include "SparseMatrixGenerator.h"


class ReservoirArp {
public:
    ReservoirArp();
    virtual ~ReservoirArp() = default;
    ReservoirNetwork reservoir;
    std::vector<std::vector<float>> readout;
    std::vector<float> readout_return;
    std::vector<float> reservoir_state;
    std::vector<float> reservoir_state_return;
    std::vector<float> output;
    int sampled_note = -1;
    
    int seed;

    
    std::default_random_engine generator;        // Random number generator


    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
    float forward(int n_notes, float softmax_beta, float feedback);
    void decode_state();
    std::vector<float> get_state();
    std::vector<float> get_readout();


    void set_sr(float sr);
    void set_lr(float lr);
    void set_units(int units);
    void set_noise_rc(float noise_rc);
    
//private:
    
};