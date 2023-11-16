#include "SparseMatrixGenerator.h"
#include <iostream>


// Constructor initializing the size N, the random seed, the distribution type, and the sparsity ratio
SparseMatrixGenerator::SparseMatrixGenerator(int N, int M, int seed, bool useNormalDistribution, float sparseRatio)
    : N(N), seed(seed), useNormalDistribution(useNormalDistribution), sparseRatio(sparseRatio) {}

// Generic distribution matrix generator
void SparseMatrixGenerator::generateMatrix(Eigen::MatrixXd& matrix, std::function<double(std::default_random_engine&)> distributionFunction) {
    // Set random seed
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution_sparse(0.0, 1.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            // Generate random number in [0, 1]
            double randomSparseValue = distribution_sparse(generator);
            
            // If the number is less than the sparsity ratio, the value is zero; otherwise, it is obtained from the distribution.
            if (randomSparseValue < sparseRatio) {
                matrix(i, j) = 0.0;
            } else {
                double randomValue = distributionFunction(generator);
                matrix(i, j) = randomValue;
            }
        }
    }
}

// Function to generate sparse matrix
Eigen::MatrixXd SparseMatrixGenerator::generateSparseMatrix() {
    Eigen::MatrixXd matrix(N, M);

    if (useNormalDistribution) {
        // Normal distribution with mean = 0 and std dev = 1
        std::normal_distribution<double> distribution(0.0, 1.0);
        generateMatrix(matrix, distribution);
    } else {
        // Bernoulli distribution with probability 0.5
        std::bernoulli_distribution distribution(0.5);
        generateMatrix(matrix, distribution);
    }

    return matrix;
}
