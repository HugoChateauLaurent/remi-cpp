#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

// Activation functions
inline std::vector<std::vector<double>> relu(const std::vector<std::vector<double>>& x) {
    std::vector<std::vector<double>> output = x;
    for (auto& row : output) {
        for (auto& val : row) {
            val = std::max(0.0, val);
        }
    }
    return output;
}

inline std::vector<std::vector<double>> sigmoid(const std::vector<std::vector<double>>& x) {
    std::vector<std::vector<double>> output = x;
    for (auto& row : output) {
        for (auto& val : row) {
            val = 1.0 / (1.0 + std::exp(-val));
        }
    }
    return output;
}

inline std::vector<std::vector<double>> tanh_func(const std::vector<std::vector<double>>& x) {
    std::vector<std::vector<double>> output = x;
    for (auto& row : output) {
        for (auto& val : row) {
            val = std::tanh(val);
        }
    }
    return output;
}
