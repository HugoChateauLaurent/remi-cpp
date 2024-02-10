/*
  ==============================================================================

    Reservoir.h
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <random>
#include "SparseMatrixGenerator.h"


class Reservoir {
public:
    Reservoir(int input_dim);
    virtual ~Reservoir() = default;
    int units = 16;
    int input_dim;
    float lr = 1.0f;
    float sr = 1.0f;
    float noise_rc = 0.0f;
    float input_scaling = 1.0f;
    float feedback_scaling = 1.0f;
    float rc_connectivity = 0.1f;
    int seed;

    std::vector<std::vector<float>> Win;
    std::vector<std::vector<float>> W;          // Reservoir matrix
    std::vector<float> state;                   // Reservoir state
    std::default_random_engine generator;        // Random number generator
    std::normal_distribution<float> distribution; // Normal distribution for noise

    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
    std::vector<float> forward(std::vector<float> x);
    std::vector<float> noise_gen(int size);

//private:
    
};