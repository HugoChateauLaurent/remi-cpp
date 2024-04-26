/*
  ==============================================================================

    ReservoirAudioFX.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirAudioFX.h"
#include "Reservoir.h"
#include <fstream>
using namespace std;


ReservoirAudioFX::ReservoirAudioFX() : reservoir(4) {
    initialize(true);
}

float ReservoirAudioFX::forward(int pattern) {
    std::vector<float> x(4);
    x[pattern] = 1;
    x[3] = feedback_mix * old_output;
    reservoir_state = reservoir.forward(x);
    reservoir_state_return = reservoir_state;
    if(readout.size() != 0)
    {
        for (int j = 0; j < reservoir.units; ++j) {
            if(readout.size() != 0)
            {
                readout_return.push_back(float(readout[j][0]));
            }
        } 
    }
    decode_state();
    
    output = 1 / (1 + exp(-output*outputGain)); // sigmoid to get [0,1]
    old_output = output;
    return static_cast<float>(output);
}

std::vector<float> ReservoirAudioFX::get_state() {

    return reservoir_state_return;
}

std::vector<float> ReservoirAudioFX::get_readout() {
    

    return readout_return;
}

void ReservoirAudioFX::decode_state() {

    // Reinitialize output
    output = 0.0f;

    for (int j = 0; j < reservoir.units; ++j) {
       
        output += readout[j][0] * reservoir_state[j] / reservoir.units;
    }
}


void ReservoirAudioFX::initialize(bool new_random_seed) {
    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);
    SparseMatrixGenerator readoutGenerator(reservoir.units, 1, seed, true, 0.0f);
    readout = readoutGenerator.generateSparseMatrix();
    output = 0.0f;
}


void ReservoirAudioFX::reset(bool new_random_seed) {
    initialize(new_random_seed);
}