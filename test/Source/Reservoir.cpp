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

Reservoir::Reservoir(int input_dim) {
    this.input_dim = input_dim;
    initialize(True);
}


void Reservoir::initialize(bool new_random_seed) {

    // Set seed for random number generation
    if (new_random_seed) {
        seed = generator.rand(999999);
    }
    Reservoir::generator.seed(seed);


    // Initialize W with your connectivity pattern
    W.resize(units, std::vector<double>(units));
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            W[i][j] = rc_connectivity * ((double)rand() / RAND_MAX - 0.5); // normal
        }
    }

    // Initialize state to zeros
    state.resize(units, 0.0);
}

void Reservoir::reset(bool new_random_seed) {
    initialize(new_random_seed);
}

void Reservoir::forward(std::vector<double> x, std::vector<double> feedback) {

    // Update reservoir state
    std::vector<double> forward_pass(units, 0.0);
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            forward_pass[i] += W[i][j] * state[j];
        }
        for (int j = 0; j < x.size; ++j) {
            forward_pass[i] += W[i][j] * x[j] * input_scaling;
        }
        for (int j = 0; j < feedback.size; ++j) {
            forward_pass[i] += W[i][j] * feedback[j] * feedback;
        }
    }
    std::vector<double> s_next(units, 0.0);
    for (int i = 0; i < units; ++i) {
        s_next[i] = (1 - lr) * state[i] + lr * std::tanh(forward_pass[i]) + noise_rc * normal_dist(generator); // normal noise - tanh(prod + b) - biais bernoulli
    }
    state = s_next;

    // Convert s_next to startWrite - sample per sample
    for (int i = 0; i < size; ++i) {
        startWrite[i] = static_cast<float>(s_next[i]);
    }
}

std::vector<double> Reservoir::noise_gen(int size) { // noise gen
    std::vector<double> noise(size);
    for (int i = 0; i < size; ++i) {
        noise[i] = distribution(generator); 
    }
    return noise;
}