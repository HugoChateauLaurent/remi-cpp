/*
  ==============================================================================

    ReservoirAudioFX.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirAudioFX.h"
#include "Reservoir.h"

ReservoirAudioFX::ReservoirAudioFX() {
    reservoir = Reservoir(2);
    initialize();
}

std::vector<double> ReservoirAudioFX::forward(const float* startRead, float* startWrite, int size) {
    std::vector<double> x(1);
    for (int i = 0; i < size; ++i) {
        x[0] = startRead[i];
        reservoir_state = reservoir::forward(x, output);
        decode_state();
        startWrite[i] = static_cast<float>(output);
    }

    return output;

}

std::vector<double> ReservoirAudioFX::decode_state() {

    // Reinitialize output
    for (int i = 0; i < output.size(); ++i) {
        output[i] = 0.0;
    }

    // 
    for (int i = 0; i < out.size(); ++i) {
        for (int j = 0; j < reservoir::units; ++j) {
            output[i] += readout[j][i] * state[j];
        }
    }
}


void ReservoirAudioFX::initialize() {
    // Initialize readout
    for (int i = 0; i < readout.size(); ++i) {
        for (int j = 0; readout[i].size(); ++j) {
            readout[i][j] = 0.0;
        }
    }
}


void ReservoirAudioFX::reset() {
    initialize();
}