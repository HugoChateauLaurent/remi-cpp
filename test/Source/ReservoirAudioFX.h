/*
  ==============================================================================

    ReservoirAudioFX.h
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#pragma once

#include "Reservoir.h"


class ReservoirAudioFX {
public:
    ReservoirAudioFX();
    virtual ~ReservoirAudioFX() = default;
    Reservoir reservoir;
    std::vector<double> reservoir_state;
    std::vector<double> output;
    std::vector<std::vector<double>> readout;

    void initialize();
    void reset();
    std::vector<double> forward(const float* startRead, float* startWrite, int size);
    std::vector<double> decode_state();

//private:
    
};