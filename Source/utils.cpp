#include "utils.h"
#include <Eigen/Sparse>
#include <iostream>
#include <random>

void initializeSparseMatrix(Eigen::SparseMatrix<float>& matrix, int rows, int cols, unsigned seed, float scaling, float connectivity_factor, const std::string& distributionType, float distributionParam1, float distributionParam2) {
    std::default_random_engine generator(seed);

    std::vector<Eigen::Triplet<float>> tripletList;

    float value;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float chance = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
            if (chance < connectivity_factor) {
                if (distributionType == "normal") {
                    std::normal_distribution<float> distribution(distributionParam1, distributionParam2);
                    value = distribution(generator) * scaling;
                } else if (distributionType == "bernoulli") {
                    std::bernoulli_distribution distribution(distributionParam1); // probability of generating 1
                    value = distribution(generator) ? scaling : -scaling;
                } else {
                    std::cerr << "Unsupported distribution type" << std::endl;
                    return;
                }
            }
            else {
                value = 0.0f;
            }
            tripletList.push_back(Eigen::Triplet<float>(i, j, value));
        }
    }

    matrix.resize(rows, cols);
    matrix.setFromTriplets(tripletList.begin(), tripletList.end());
}