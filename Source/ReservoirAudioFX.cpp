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

std::vector<float> ReservoirAudioFX::forward(int pattern) {
    reservoir_state.clear();
    for(int i = 0; i < old_output.size(); i++)
    {
        
        std::vector<float> x(4);
        x[pattern] = 1;
        x[3] = feedback_mix * old_output[i];
        reservoir_state.push_back(reservoir.forward(x));
        //reservoir_state_return = reservoir_state;
        
        output[i] = decode_state(i,reservoir_state[i]);
    
        output[i] = 1 / (1 + exp(-output[i]*outputGain)); // sigmoid to get [0,1]
        old_output[i] = output[i];
    }

    return output;
}

std::vector<float> ReservoirAudioFX::get_state() {

    return reservoir_state_return;
}

std::vector<float> ReservoirAudioFX::get_readout() {
    

    return readout_return;
}

float ReservoirAudioFX::decode_state(int i, std::vector<float> reservoir_state) {

    // Reinitialize output
    float out = 0.0f;
    if(reservoir.units < reservoir_state.size())
    {
        for (int j = 0; j < reservoir.units; ++j) {
       
            out += readout[j][i] * reservoir_state[j] / reservoir.units;
        }
    
    }
    else
    {

        for (int j = 0; j < reservoir_state.size(); ++j) {
       
            out += readout[j][i] * reservoir_state[j] / reservoir.units;
        }
    }
    return out;
}


void ReservoirAudioFX::initialize(bool new_random_seed) {
    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);
    SparseMatrixGenerator readoutGenerator(reservoir.units, 3, seed, true, 0.0f);//
    readout_return.clear();
    readout = readoutGenerator.generateSparseMatrix();
    /*
    for (int j = 0; j < reservoir.units; ++j) 
    {
        if(readout.size() != 0)
        {
            readout_return.push_back(float(readout[j][0]));
        }
    } 
    */
    output.resize(3, 0.0f);
    old_output.resize(3, 0.0f);
}


void ReservoirAudioFX::reset(bool new_random_seed) {
    initialize(new_random_seed);
}