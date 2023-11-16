#ifndef SPARSE_MATRIX_GENERATOR_H
#define SPARSE_MATRIX_GENERATOR_H

#include <vector>
#include <functional>
#include <random>
#include "./Eigen/Dense"

class SparseMatrixGenerator {
public:
    // Constructeur prenant la taille N, la graine aléatoire, le type de distribution et le ratio de sparsité
    SparseMatrixGenerator(int N, int seed, bool useNormalDistribution, float sparseRatio);

    // Fonction pour générer une matrice sparse
    Eigen::MatrixXd generateSparseMatrix();

private:
    int N;
    int seed;
    bool useNormalDistribution;
    float sparseRatio;

    // Fonction générique pour la génération de la matrice en fonction de la distribution
    void generateMatrix(Eigen::MatrixXd& matrix, std::function<double(std::default_random_engine&)> distributionFunction);
};

#endif // SPARSE_MATRIX_GENERATOR_H
