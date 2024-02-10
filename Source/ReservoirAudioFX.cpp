/*
  ==============================================================================

    ReservoirAudioFX.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirAudioFX.h"
#include "Reservoir.h"

ReservoirAudioFX::ReservoirAudioFX() : reservoir(2) {
    initialize(true);
}

float ReservoirAudioFX::forward(float sample) {
    std::vector<float> x(2);
    x[0] = (1-feedback_mix) * sample;
    x[1] = feedback_mix * old_output;
    reservoir_state = reservoir.forward(x);
    decode_state();
    old_output = output;
    return outputGain*static_cast<float>(output);
}

void ReservoirAudioFX::decode_state() {

    // Reinitialize output
    output = 0.0f;

    for (int j = 0; j < reservoir.units; ++j) {
        output += readout[j][0] * reservoir_state[j];
    }
}


void ReservoirAudioFX::initialize(bool new_random_seed) {
    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);
    SparseMatrixGenerator readoutGenerator(reservoir.units, 1, seed, false, 0.0f);
    readout = readoutGenerator.generateSparseMatrix();
    output = 0.0f;
}


void ReservoirAudioFX::reset(bool new_random_seed) {
    initialize(new_random_seed);
}