#ifndef SPARSE_MATRIX_GENERATOR_H
#define SPARSE_MATRIX_GENERATOR_H

#include <vector>
#include <functional>
#include <random>

class SparseMatrixGenerator {
public:
    // Constructeur prenant la taille N, la graine aléatoire, le type de distribution et le ratio de sparsité
    SparseMatrixGenerator(int N, int seed, bool useNormalDistribution, float sparseRatio);

    // Fonction pour générer une matrice sparse
    std::vector<std::vector<double>> generateSparseMatrix();

private:
    int N;
    int seed;
    bool useNormalDistribution;
    float sparseRatio;

    // Fonction générique pour la génération de la matrice en fonction de la distribution
    void generateMatrix(std::vector<std::vector<double>>& matrix, std::function<double(std::default_random_engine&)> distributionFunction);
};

#endif // SPARSE_MATRIX_GENERATOR_H
