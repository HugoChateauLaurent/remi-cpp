/*
  ==============================================================================

    ReservoirAudioFX.h
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#pragma once


class ReservoirAudioFX {
public:
    ReservoirAudioFX();
    virtual ~ReservoirAudioFX() = default;
    Reservoir reservoir;

    int units = 1;
    float lr = 1.0;
    float sr = 1.0;
    float noise_rc = 0.0;
    float input_scaling = 1.0;
    double rc_connectivity = 0.1;
    int seed;
    

    void initialize();
    void reset();
    void forward(const float* startRead, float* startWrite, int size);


//private:
    
};