/*
  ==============================================================================

    Reservoir.cpp
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#include "Reservoir.h"

#include <iostream>
#include <Eigen/Dense>
#include <random>

using namespace Eigen;

class Reservoir {
public:
    int units;           // Number of neurons.
    float lr;            // Leaking rate (1.0 by default)
    float sr;            // Spectral Radius (1.0 by default).
    float noise_rc;      // Reservoir state noise gain (0 by default).
    float input_scaling; // Input scaling (1.0 by default).
    float rc_connectivity; // Connectivity (or density) of ``W`` (0.1 by default).
    int seed;            // Seed for random number generation.
    MatrixXd W;          // Reservoir matrix
    VectorXd state;      // Reservoir state
    std::default_random_engine generator; // Random number generator
    std::normal_distribution<double> distribution; // Normal distribution for noise

    Reservoir(int units, float lr = 1.0, float sr = 1.0, float noise_rc = 0.0, float input_scaling = 1.0,
              float rc_connectivity = 0.1, int seed = 0)
        : units(units), lr(lr), sr(sr), noise_rc(noise_rc), input_scaling(input_scaling),
          rc_connectivity(rc_connectivity), seed(seed),
          W(units, units), state(units), distribution(0.0, 1.0) {
        initialize();
    }

    void initialize() {
        // Set seed for random number generation
        generator.seed(seed);

        // Initialize W with your connectivity pattern
        W = MatrixXd::Random(units, units);
        W = rc_connectivity * W / W.norm(); // Normalize to achieve the desired connectivity

        // Initialize state to zeros
        state.setZero();
    }

    VectorXd forward(VectorXd x) {
        // Apply input scaling
        x *= input_scaling;

        // Update reservoir state
        VectorXd prod = W * state;
        VectorXd s_next = (1 - lr) * state + lr * prod + noise_rc * noise_gen(state.size());
        state = s_next;

        return s_next;
    }

    VectorXd noise_gen(int size) {
        VectorXd noise(size);
        for (int i = 0; i < size; ++i) {
            noise[i] = distribution(generator);
        }
        return noise;
    }
};