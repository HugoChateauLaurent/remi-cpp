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
    std::vector<std::vector<double>> readout(1, std::vector<double>(1, 0.0));
    std::vector<double> reservoir_state;
    std::vector<double>(1, 0.0) output;

    void initialize();
    void reset();
    std::vector<double> forward(const float* startRead, float* startWrite, int size);

//private:
    
};