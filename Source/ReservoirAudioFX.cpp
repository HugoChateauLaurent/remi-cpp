/*
  ==============================================================================

    ReservoirAudioFX.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirAudioFX.h"
#include "Reservoir.h"
#include "utils.h"

ReservoirAudioFX::ReservoirAudioFX() : reservoir(1, 1, 1.0f, 1.0f) {
    initialize(true);
}

float ReservoirAudioFX::forward(float sample) {
    Eigen::VectorXf x = Eigen::VectorXf::Constant(1, sample);
    reservoir_state = reservoir.forward(x,  Eigen::VectorXf::Constant(1, output));
    decode_state();
    return outputGain*static_cast<float>(output);
}

void ReservoirAudioFX::decode_state() {

    // Reinitialize output
    Eigen::VectorXf output_vector = readout * reservoir_state;

    output = output_vector(0);
}


void ReservoirAudioFX::initialize(bool new_random_seed) {
    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);
    initializeSparseMatrix(readout, 1, reservoir.units, seed, 1.0f, readout_connectivity, "bernoulli", 0.5f, 0.0f);
    output = 0.0f;
}


void ReservoirAudioFX::reset(bool new_random_seed) {
    initialize(new_random_seed);
}