#pragma once

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <random>
#include "ActivationFunctions.h"

// Linear layer class
class Linear {
public:
    std::vector<std::vector<double>> weight; // 2D vector for weights
    std::vector<double> bias;                // 1D vector for biases

    Linear(int input_size, int output_size) {
        // Use a fixed seed for the random number generator
        static std::default_random_engine generator(42); // Fixed seed
        std::normal_distribution<double> distribution(0.0, 1.0);

        // Initialize weights
        weight.resize(output_size, std::vector<double>(input_size));
        for (int i = 0; i < output_size; ++i) {
            for (int j = 0; j < input_size; ++j) {
                weight[i][j] = distribution(generator);
            }
        }

        // Initialize biases
        bias.resize(output_size);
        for (int i = 0; i < output_size; ++i) {
            bias[i] = distribution(generator);
        }
    }

    // Forward pass through the linear layer
    std::vector<std::vector<double>> forward(const std::vector<std::vector<double>>& x) {
        size_t batch_size = x.size();
        int input_size = weight[0].size();
        int output_size = weight.size();

        std::vector<std::vector<double>> output(batch_size, std::vector<double>(output_size));

        for (size_t n = 0; n < batch_size; ++n) {
            for (int i = 0; i < output_size; ++i) {
                double sum = bias[i];
                for (int j = 0; j < input_size; ++j) {
                    sum += x[n][j] * weight[i][j];
                }
                output[n][i] = sum;
            }
        }

        return output;
    }
};

// Neural network for color generation
class ColorNet {
public:
    std::vector<Linear> layers;
    std::vector<std::function<std::vector<std::vector<double>>(const std::vector<std::vector<double>>&)>>
        activations;

    ColorNet(int input_size, const std::vector<int>& hidden_sizes,
             const std::vector<std::string>& activation_functions) {
        // Map activation function names to actual functions
        static std::map<std::string,
                        std::function<std::vector<std::vector<double>>(const std::vector<std::vector<double>>&)>>
            activation_map = {
                {"relu", relu},
                {"sigmoid", sigmoid},
                {"tanh", tanh_func},
                {"sinusoid", sinusoid}};

        int current_size = input_size;

        // Initialize layers and activation functions
        for (size_t i = 0; i < hidden_sizes.size(); ++i) {
            layers.emplace_back(current_size, hidden_sizes[i]);
            activations.push_back(activation_map[activation_functions[i]]);
            current_size = hidden_sizes[i];
        }

        // Output layer with 3 units for RGB values
        layers.emplace_back(current_size, 3);
    }

    // Forward pass through the network
    std::vector<std::vector<double>> forward(const std::vector<std::vector<double>>& x) {
        std::vector<std::vector<double>> output = x;
        for (size_t i = 0; i < layers.size() - 1; ++i) {
            output = layers[i].forward(output);
            output = activations[i](output);
        }
        output = layers.back().forward(output);
        output = sigmoid(output);  // Output RGB values between 0 and 1
        return output;
    }
};
