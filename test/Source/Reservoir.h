/*
  ==============================================================================

    Reservoir.h
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <eigen3/Eigen/Dense>
#include <random>


class Reservoir {
public:
    Reservoir() = default;
    virtual ~Reservoir() = default;
    int units;
    float lr = 1.0;
    float sr = 1.0;
    float noise_rc = 0.0;
    float input_scaling = 1.0;
    double rc_connectivity = 0.1;
    int seed = 0;

    std::vector<std::vector<double>> W;          // Reservoir matrix
    std::vector<double> state;                   // Reservoir state
    std::default_random_engine generator;        // Random number generator
    std::normal_distribution<double> distribution; // Normal distribution for noise

    void initialize();
    void forward(const float* startRead, float* startWrite, int size);
    std::vector<double> noise_gen(int size);

//private:
    
};

