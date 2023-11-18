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
    Reservoir::generator.seed(seed);

    SparseMatrixGenerator initGenerator(1, 1, seed, true, sparseRatio); // normal distibution
    initNormalMatrix = initGenerator.generateSparseMatrix();

    SparseMatrixGenerator forwardGenerator(1, 1, seed+1, true, sparseRatio); // normal distibution
    forwardNormalMatrix = forwardGenerator.generateSparseMatrix();

    // Initialize W with your connectivity pattern
    W.resize(units, std::vector<double>(units));
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            W[i][j] = rc_connectivity * initNormalMatrix[i][j]; // normal
        }
    }

    // Initialize state to zeros
    state.resize(units, 0.0);
}

void Reservoir::reset(bool new_random_seed) {
    initialize(new_random_seed);
}

std::vector<double> Reservoir::forward(std::vector<double> x, std::vector<double> feedback) {

    // Update reservoir state
    std::vector<double> forward_pass(units, 0.0); // TODO: remove memory allocation in real-time loop
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            forward_pass[i] += W[j][i] * state[j];
        }
        for (int j = 0; j < x.size(); ++j) {
            forward_pass[i] += W[i][j] * x[j] * input_scaling;
        }
        for (int j = 0; j < feedback.size(); ++j) {
            forward_pass[i] += W[i][j] * feedback[j] * feedback_scaling;
        }
    }
    std::vector<double> s_next(units, 0.0); // TODO: remove memory allocation in real-time loop
    for (int i = 0; i < units; ++i) {
        s_next[i] = (1 - lr) * state[i] + lr * std::tanh(forward_pass[i]) + noise_rc * forwardNormalMatrix[i][0]; // normal noise - tanh(prod + b) - biais bernoulli
    }
    state = s_next;

    return state;
}

std::vector<double> Reservoir::noise_gen(int size) { // noise gen
    std::vector<double> noise(size);
    for (int i = 0; i < size; ++i) {
        noise[i] = forwardNormalMatrix[i][0];  // placeholder noise
    }
    return noise;
}