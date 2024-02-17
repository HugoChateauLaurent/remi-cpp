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
#include <Eigen/Sparse>



class Reservoir {
public:
    Reservoir(int input_dim_value, int fb_dim_value, float input_connectivity_value, float fb_connectivity_value);
    virtual ~Reservoir() = default;
    int units = 16;
    int input_dim;
    int fb_dim;
    float lr = 1.0f;
    float sr = 1.0f;
    float input_scaling = 1.0f;
    float fb_scaling = 1.0f;
    float rc_connectivity = 0.1f;
    float input_connectivity = 0.1f;
    float fb_connectivity = 0.1f;
    unsigned seed;

    Eigen::SparseMatrix<float> Win;
    Eigen::SparseMatrix<float> W;          // Reservoir matrix
    Eigen::SparseMatrix<float> Wfb;         
    Eigen::VectorXf state;                   // Reservoir state

    std::default_random_engine generator;        // Random number generator

    void initialize(bool new_random_seed);
    void reset(bool new_random_seed);
Eigen::VectorXf Reservoir::forward(Eigen::VectorXf x, Eigen::VectorXf fb);
//private:
    
};