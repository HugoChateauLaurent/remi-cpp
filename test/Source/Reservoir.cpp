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

class Reservoir {
public:
    Reservoir();

    void initialize();

    void forward(float* startRead, float* startWrite, int size);

    std::vector<double> noise_gen(int size);

private:
    int seed;
    int units;
    double rc_connectivity;
    double input_scaling;
    double lr;
    double noise_rc;

    std::vector<std::vector<double>> W;          // Reservoir matrix
    std::vector<double> state;                   // Reservoir state
    std::default_random_engine generator;        // Random number generator
    std::normal_distribution<double> distribution; // Normal distribution for noise
};

Reservoir::Reservoir() {}

void Reservoir::initialize() {
    // Set seed for random number generation
    generator.seed(seed);

    // Initialize W with your connectivity pattern
    W.resize(units, std::vector<double>(units));
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            W[i][j] = rc_connectivity * ((double)rand() / RAND_MAX - 0.5);
        }
    }

    // Normalize to achieve the desired connectivity
    double norm = 0.0;
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            norm += W[i][j] * W[i][j];
        }
    }
    norm = sqrt(norm);
    if (norm > 0.0) {
        for (int i = 0; i < units; ++i) {
            for (int j = 0; j < units; ++j) {
                W[i][j] /= norm;
            }
        }
    }

    // Initialize state to zeros
    state.resize(units, 0.0);
}

void Reservoir::forward(float* startRead, float* startWrite, int size) {
    std::vector<double> x(size);
    for (int i = 0; i < size; ++i) {
        x[i] = startRead[i];
    }

    // Apply input scaling
    for (double& val : x) {
        val *= input_scaling;
    }

    // Update reservoir state
    std::vector<double> prod(units, 0.0);
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            prod[i] += W[i][j] * state[j];
        }
    }
    std::vector<double> s_next(units, 0.0);
    for (int i = 0; i < units; ++i) {
        s_next[i] = (1 - lr) * state[i] + lr * prod[i] + noise_rc * distribution(generator);
    }
    state = s_next;

    // Convert s_next to startWrite - sample per sample
    for (int i = 0; i < size; ++i) {
        startWrite[i] = static_cast<float>(s_next[i]);
    }
}

std::vector<double> Reservoir::noise_gen(int size) {
    std::vector<double> noise(size);
    for (int i = 0; i < size; ++i) {
        noise[i] = distribution(generator);
    }
    return noise;
}