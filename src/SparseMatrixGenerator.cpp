#include "SparseMatrixGenerator.h"
#include <iostream>

// Constructeur initialisant la taille N, la graine aléatoire, le type de distribution et le ratio de sparsité
SparseMatrixGenerator::SparseMatrixGenerator(int N, int seed, bool useNormalDistribution, float sparseRatio)
    : N(N), seed(seed), useNormalDistribution(useNormalDistribution), sparseRatio(sparseRatio) {}

// Fonction générique pour la génération de la matrice en fonction de la distribution
void SparseMatrixGenerator::generateMatrix(std::vector<std::vector<double>>& matrix, std::function<double(std::default_random_engine&)> distributionFunction) {
    // Initialisation du générateur de nombres aléatoires avec la graine spécifiée
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution_sparse(0.0, 1.0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            // Génère un nombre aléatoire entre 0 et 1
            double randomSparseValue = distribution_sparse(generator);
            
            // Si le nombre est inférieur au ratio de sparsité, la valeur est nulle, sinon elle est obtenue par la distribution
            if (randomSparseValue < sparseRatio) {
                matrix[i][j] = 0.0;
            } else {
                double randomValue = distributionFunction(generator);
                matrix[i][j] = randomValue;
            }
        }
    }
}

// Fonction pour générer une matrice sparse
std::vector<std::vector<double>> SparseMatrixGenerator::generateSparseMatrix() {
    std::vector<std::vector<double>> matrix(N, std::vector<double>(N, 0.0));

    if (useNormalDistribution) {
        // Distribution normale avec une moyenne de 0 et un écart-type de 1
        std::normal_distribution<double> distribution(0.0, 1.0);
        generateMatrix(matrix, distribution);
    } else {
        // Distribution de Bernoulli avec une probabilité de 0.5
        std::bernoulli_distribution distribution(0.5);
        generateMatrix(matrix, distribution);
    }

    return matrix;
}
