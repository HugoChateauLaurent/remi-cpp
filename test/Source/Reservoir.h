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
    int units = 1;
    int input_dim;
    float lr = 1.0;
    float sr = 1.0;
    float noise_rc = 0.0;
    float input_scaling = 1.0;
    float feedback_scaling = 1.0;
    double rc_connectivity = 0.1;
    double sparseRatio = 0.1;
    int seed;

    std::vector<std::vector<double>> initNormalMatrix;
    std::vector<std::vector<double>> forwardNormalMatrix;
    std::vector<std::vector<double>> W;          // Reservoir matrix
    std::vector<double> state;                   // Reservoir state
    std::default_random_engine generator;        // Random number generator
    std::normal_distribution<double> distribution; // Normal distribution for noise

    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
    std::vector<double> forward(std::vector<double> x, std::vector<double> feedback);
    std::vector<double> noise_gen(int size);

//private:
    
};

