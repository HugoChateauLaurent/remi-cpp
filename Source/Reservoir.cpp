/*  
  ==============================================================================

    Reservoir.cpp
    Created: 16 Nov 2023 1:31:21pm
    Author:  HomePC

  ==============================================================================
*/

#include "Reservoir.h"
#include "utils.h"

#include <iostream>
#include <random>
#include <Eigen/Sparse>




Reservoir::Reservoir(int input_dim_value, int fb_dim_value, float input_connectivity_value, float fb_connectivity_value) {
    input_dim = input_dim_value;
    fb_dim = fb_dim_value;
    input_connectivity = input_connectivity_value;
    fb_connectivity = fb_connectivity_value;
    initialize(true);
}


void Reservoir::initialize(bool new_random_seed) {

    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);

    initializeSparseMatrix(W, units, units, seed, 1.0f, rc_connectivity, "normal", 0.0f, 1.0f);
    initializeSparseMatrix(Win, units, input_dim, seed, 1.0f, input_connectivity, "bernoulli", 0.5f, 0.0f);
    initializeSparseMatrix(Wfb, units, fb_dim, seed, 1.0f, 1.0f, "bernoulli", 0.5f, 0.0f);


    // Initialize state to zeros
    state.resize(units);
    state.setConstant(0.0f);
}

void Reservoir::reset(bool new_random_seed) {
    initialize(new_random_seed);
}


// std::vector<float> Reservoir::forward(std::vector<float> x) {

//     // Update reservoir state
//     std::vector<float> forward_pass(units, 0.0f);
//     for (int i = 0; i < units; ++i) {
//         for (int j = 0; j < units; ++j) {
//             forward_pass[i] += W[j][i] * sr * state[j];
//         }
//         for (int j = 0; j < x.size(); ++j) {
//             forward_pass[i] += Win[j][i] * x[j] * input_scaling;
//         }
//     }
//     std::vector<float> s_next(units, 0.0);
    
//     for (int i = 0; i < units; ++i) {
//         s_next[i] = (1 - lr) * state[i] + lr * std::tanh(forward_pass[i]); // normal noise - tanh(prod + b) - biais bernoulli
//     }
//     state = s_next;

//     return state;
// }

Eigen::VectorXf Reservoir::forward(Eigen::VectorXf x, Eigen::VectorXf fb) {

    Eigen::VectorXf recurrence = W * (sr * state);
    Eigen::VectorXf input = Win * (x * input_scaling);
    Eigen::VectorXf feedback = Wfb * (fb * fb_scaling);

    Eigen::VectorXf forward_pass = recurrence + input + feedback;

    // Next state calculation
    Eigen::VectorXf s_next = (1 - lr) * state + lr * forward_pass.unaryExpr([](float val) { return std::tanh(val); });

    // Update the state
    state = s_next;
    
    return state;
}