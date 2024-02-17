#ifndef UTILS_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define UTILS_H

#include <Eigen/Sparse>
#include <iostream>
#include <random>

void initializeSparseMatrix(Eigen::SparseMatrix<float>& matrix, int rows, int cols, unsigned seed, float scaling, float connectivity_factor, const std::string& distributionType, float distributionParam1, float distributionParam2);

#endif
