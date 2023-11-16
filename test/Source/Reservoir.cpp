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


Eigen::MatrixXd W;          // Reservoir matrix
Eigen::VectorXd state;      // Reservoir state
std::default_random_engine generator; // Random number generator
std::normal_distribution<double> distribution; // Normal distribution for noise

Reservoir::Reservoir() {

}

void Reservoir::initialize() {
    // Set seed for random number generation
    generator.seed(seed);

    // Initialize W with your connectivity pattern
    W = Eigen::MatrixXd::Random(units, units);
    W = rc_connectivity * W / W.norm(); // Normalize to achieve the desired connectivity

    // Initialize state to zeros
    state.setZero();
}

void Reservoir::forward(float* startRead, float* startWrite, int size) {
    Eigen::VectorXd x(startRead, startRead + size);
    // Apply input scaling
    x *= input_scaling;

    // Update reservoir state
    Eigen::VectorXd prod = W * state;
    Eigen::VectorXd s_next = (1 - lr) * state + lr * prod + noise_rc * noise_gen(state.size());
    state = s_next;
        
    // convert s_next to startWrite - sample per sample
    for (int i = 0; i < size; ++i) {
        startWrite[i] = s_next[i];
    }

    //return s_next;
}

Eigen::VectorXd Reservoir::noise_gen(int size) {
    Eigen::VectorXd noise(size);
    for (int i = 0; i < size; ++i) {
        noise[i] = distribution(generator);
    }
    return noise;
};