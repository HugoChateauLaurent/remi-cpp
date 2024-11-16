#pragma once

#include <torch/torch.h>
#include <map>
#include <vector>
#include <functional>

// Neural network for color generation
struct ColorNet : torch::nn::Module {
    std::vector<torch::nn::Linear> layers;
    std::vector<std::function<torch::Tensor(const torch::Tensor&)>> activations;

    ColorNet(int input_size, const std::vector<int>& hidden_sizes, const std::vector<std::string>& activation_functions) {
        static std::map<std::string, std::function<torch::Tensor(const torch::Tensor&)>> activation_map = {
            {"relu", torch::relu},
            {"sigmoid", torch::sigmoid},
            {"tanh", torch::tanh}
        };

        int current_size = input_size;

        for (size_t i = 0; i < hidden_sizes.size(); ++i) {
            layers.push_back(register_module("layer" + std::to_string(i), torch::nn::Linear(current_size, hidden_sizes[i])));
            activations.push_back(activation_map[activation_functions[i]]);
            current_size = hidden_sizes[i];
        }

        // Output layer with 3 units for RGB values
        layers.push_back(register_module("output", torch::nn::Linear(current_size, 3)));
    }

    torch::Tensor forward(torch::Tensor x) {
        for (size_t i = 0; i < layers.size() - 1; ++i) {
            x = activations[i](layers[i]->forward(x));
        }
        return torch::sigmoid(layers.back()->forward(x)); // Output RGB between 0 and 1
    }
};
