/*
  ==============================================================================

    Reservoir.h
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <random>

class Reservoir
{
public:
    Reservoir() = default;

    void initialize();
    VectorXd forward(VectorXd x);
    VectorXd noise_gen(int size); 

    void process(const float* inData, float* outData, int numSamples);
    // void process(const float* inData, float param1, float param2, float* outData, int numSamples);
// private:
    
};