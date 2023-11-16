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

class Reservoir
{
public:
    Reservoir();
    //Reservoir(int units, float lr, float sr, float noise_rc, float input_scaling, float rc_connectivity, int seed);

    void initialize();
    void forward(float* startRead, float* startWrite, int size);
    Eigen::VectorXd noise_gen(int size);

    int units;
    float lr = 1.0; 
    float sr = 1.0; 
    float noise_rc = 0.0; 
    float input_scaling = 1.0;
    double rc_connectivity = 0.1; 
    int seed = 0;
    /*int units;           // Number of neurons.
    float lr;            // Leaking rate (1.0 by default)
    float sr;            // Spectral Radius (1.0 by default).
    float noise_rc;      // Reservoir state noise gain (0 by default).
    float input_scaling; // Input scaling (1.0 by default).
    float rc_connectivity; // Connectivity (or density) of ``W`` (0.1 by default).
    int seed;            // Seed for random number generation.*/


// private:
    
};