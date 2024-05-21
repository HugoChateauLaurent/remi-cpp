#include "SparseMatrixGenerator.h"
#include <iostream>


// Fonction générique pour la génération de la matrice en fonction de la distribution
void SparseMatrixGenerator::generateMatrix(std::vector<std::vector<float>>& matrix, std::function<float(std::default_random_engine&)> distributionFunction) {
    // Initialisation du générateur de nombres aléatoires avec la graine spécifiée
    std::default_random_engine generator(seed);
    std::uniform_real_distribution<float> distribution_sparse(0.0f, 1.0f);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            // Génère un nombre aléatoire entre 0 et 1
            float randomSparseValue = distribution_sparse(generator);

            // Si le nombre est inférieur au ratio de sparsité, la valeur est nulle, sinon elle est obtenue par la distribution
            if (randomSparseValue < sparseRatio) {
                matrix[i][j] = 0.0f;
            }
            else {
                float randomValue = distributionFunction(generator);
                matrix[i][j] = randomValue * 2 - 1;
            }
        }
    }
}

// Fonction pour générer une matrice sparse
std::vector<std::vector<float>> SparseMatrixGenerator::generateSparseMatrix() {
    std::vector<std::vector<float>> matrix(N, std::vector<float>(M, 0.0f));


    if (useNormalDistribution) {
        // Distribution normale avec une moyenne de 0 et un écart-type de 1
        std::normal_distribution<float> distribution(0.0f, 1.0f);
        generateMatrix(matrix, distribution);
    }
    else {
        // Distribution de Bernoulli avec une probabilité de 0.5
        std::bernoulli_distribution distribution(0.5f);
        generateMatrix(matrix, distribution);
    }

    return matrix;
}