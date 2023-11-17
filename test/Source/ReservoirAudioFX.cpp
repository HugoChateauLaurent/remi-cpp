/*
  ==============================================================================

    ReservoirAudioFX.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirAudioFX.h"
#include "Reservoir.h"

void ReservoirAudioFX::ReservoirAudioFX() {
    reservoir = Reservoir(2);
    initialize();
}

void ReservoirAudioFX::forward(const float* startRead, float* startWrite, int size) {
    std::vector<double> x(size);
    for (int i = 0; i < size; ++i) {
        x[i] = startRead[i];
    }

    reservoir_state = reservoir.forward(x, output);

    decode_state();

    return out;

}

std::vector<double> ReservoirAudioFX::decode_state() {
    for (int i = 0; i < units; ++i) {
        for (int j = 0; j < units; ++j) {
            forward_pass[i] += W[i][j] * state[j];
        }
    }
}


void ReservoirAudioFX::initialize() {
    // TODO initialize readout and feedback weights
    readout = ;
    feedback;
}