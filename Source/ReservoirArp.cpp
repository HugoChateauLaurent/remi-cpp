/*
  ==============================================================================

    ReservoirArp.cpp
    Created: 17 Nov 2023 8:19:11am
    Author:  HomePC

  ==============================================================================
*/

#include "ReservoirArp.h"
#include "Reservoir.h"
#include <fstream>
#include <numeric>
using namespace std;


ReservoirArp::ReservoirArp() : reservoir(10) {
    initialize(true);
}

float ReservoirArp::forward(int n_notes, float softmax_beta, float feedback) {
    std::vector<float> x(10);
    if (sampled_note != -1)
    {
        x[sampled_note] = feedback;
    }
    reservoir_state = reservoir.forward(x);
    reservoir_state_return = reservoir_state;
    
    decode_state();

    // Compute the softmax over the n_notes first dimensions of output, others get zero
    std::vector<float> softmax_output(output.size(), 0.0f);
    float max_val = *std::max_element(output.begin(), output.begin() + n_notes);
    float sum = 0.0f;
    for (int i = 0; i < n_notes; ++i) {
        softmax_output[i] = std::exp(softmax_beta * (output[i] - max_val));
        sum += softmax_output[i];
    }
    for (int i = 0; i < n_notes; ++i) {
        softmax_output[i] /= sum;
    }

    // Compute the cumulative distribution
    std::vector<float> cumulative_distribution(n_notes);
    std::partial_sum(softmax_output.begin(), softmax_output.begin() + n_notes, cumulative_distribution.begin());

    // Generate a random number between 0 and 1
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    float random_value = dis(generator);

    // Find the index where the random value falls in the cumulative distribution
    auto it = std::lower_bound(cumulative_distribution.begin(), cumulative_distribution.end(), random_value);
    sampled_note = std::distance(cumulative_distribution.begin(), it);
    
    return sampled_note;
}

std::vector<float> ReservoirArp::get_state() {

    return reservoir_state_return;
}

std::vector<float> ReservoirArp::get_readout() {
    

    return readout_return;
}

void ReservoirArp::decode_state() {

    // Reinitialize output with zeros
    output = std::vector<float>(10, 0.0f);

    for (int j = 0; j < reservoir_state.size(); ++j) {
        for (int i = 0; i < 10; ++i) {
            output[i] += readout[j][i] * reservoir_state[j] / reservoir.units;
        }
    }
}


void ReservoirArp::initialize(bool new_random_seed) {
    // Set seed for random number generation
    if (new_random_seed) {
        seed = rand(); // generate random seed
        srand(seed); // set seed of random generation
    }
    generator.seed(seed);
    SparseMatrixGenerator readoutGenerator(reservoir.units, 10, seed, true, 0.0f);
    readout_return.clear();
    readout = readoutGenerator.generateSparseMatrix();

    output = std::vector<float>(10, 0.0f);
}


void ReservoirArp::reset(bool new_random_seed) {
    initialize(new_random_seed);
}