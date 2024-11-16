#pragma once

#include <vector>
#include <cmath>
#include <random>

// Generate input features for a given grid cell
inline std::vector<double> generateInputFeatures(
    int x, int y, int grid_width, int grid_height, const std::vector<double>& random_vars) {
    std::vector<double> features;

    // Normalize x and y coordinates
    features.push_back(static_cast<double>(x) / (grid_width - 1));
    features.push_back(static_cast<double>(y) / (grid_height - 1));

    // Compute distance from center
    double center_x = (grid_width - 1) / 2.0;
    double center_y = (grid_height - 1) / 2.0;
    double distance = std::sqrt(std::pow(x - center_x, 2) + std::pow(y - center_y, 2)) /
                      std::sqrt(std::pow(center_x, 2) + std::pow(center_y, 2));
    features.push_back(distance);

    // Add random variables (same for all pixels in the frame)
    for (const auto& rand_var : random_vars) {
        features.push_back(rand_var);
    }

    return features;
}