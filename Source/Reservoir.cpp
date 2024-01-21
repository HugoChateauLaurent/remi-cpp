/*  
  ==============================================================================

    Reservoir.cpp
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#include "Reservoir.h"

#include <iostream>
#include <random>

Reservoir::Reservoir(int input_dim_value) {
    input_dim = input_dim_value;
    initialize(true);
}


void Reservoir::initialize(bool new_random_seed) {

    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);

    SparseMatrixGenerator WGenerator(units, units, seed, true, 1.0f-rc_connectivity); // normal distibution
    W = WGenerator.generateSparseMatrix();

    SparseMatrixGenerator WinGenerator(input_dim, units, seed+1, false, 0.0f); // normal distibution
    Win = WinGenerator.generateSparseMatrix();

    // Initialize state to zeros
    state.resize(units, 0.0f);
}

void Reservoir::reset(bool new_random_seed) {
    initialize(new_random_seed);
}


std::vector<float> Reservoir::forward(std::vector<float> x) {

    // TODO: add noise

    // Update reservoir state
    std::vector<float> forward_pass(units, 0.0f);
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            forward_pass[i] += W[j][i] * state[j];
        }
        for (int j = 0; j < x.size(); ++j) {
            forward_pass[i] += Win[j][i] * x[j] * input_scaling;
        }
    }
    std::vector<float> s_next(units, 0.0);
    
    for (int i = 0; i < units; ++i) {
        s_next[i] = (1 - lr) * state[i] + lr * std::tanh(forward_pass[i]); // normal noise - tanh(prod + b) - biais bernoulli
    }
    state = s_next;

    return state;
}

std::vector<float> Reservoir::noise_gen(int size) { // noise gen
    std::vector<float> noise(size);
    for (int i = 0; i < size; ++i) {
        noise[i] = distribution(generator); 
    }
    return noise;
}