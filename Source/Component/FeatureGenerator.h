#pragma once

#include <vector>
#include <cmath>
#include <random>

// Generate input features for a given grid cell
std::vector<float> generateInputFeatures(int x, int y, int grid_width, int grid_height, int num_random_vars) {
    float norm_x = static_cast<float>(x) / grid_width;
    float norm_y = static_cast<float>(y) / grid_height;

    float center_x = 0.5f;
    float center_y = 0.5f;
    float distance_to_center = std::sqrt(std::pow(norm_x - center_x, 2) + std::pow(norm_y - center_y, 2));

    std::vector<float> features = {norm_x, norm_y, distance_to_center};

    // Generate random variables
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < num_random_vars; ++i) {
        features.push_back(dis(gen));
    }

    return features;
}
