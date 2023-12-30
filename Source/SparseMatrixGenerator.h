#ifndef SPARSE_MATRIX_GENERATOR_H
#define SPARSE_MATRIX_GENERATOR_H

#include <vector>
#include <functional>
#include <random>
#include <Eigen/Dense>

class SparseMatrixGenerator {
public:
    // Constructor initializing the size NxM, the random seed, the distribution type, and the sparsity ratio
    SparseMatrixGenerator(int N, int M, int seed, bool useNormalDistribution, float sparseRatio);

    ~SparseMatrixGenerator() = default;

    // Create sparse matrix
    Eigen::MatrixXd generateSparseMatrix();

private:
    int N;
    int M;
    int seed;
    bool useNormalDistribution;
    float sparseRatio;

    // Generate distribution matrix
    void generateMatrix(Eigen::MatrixXd& matrix, std::function<double(std::default_random_engine&)> distributionFunction);
};

#endif // SPARSE_MATRIX_GENERATOR_H
