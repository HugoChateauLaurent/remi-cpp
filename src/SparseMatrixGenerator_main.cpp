#include <iostream>
#include "SparseMatrixGenerator.h"

int main() {
    // Taille de la matrice
    int N;
    std::cout << "Entrez la taille de la matrice : ";
    std::cin >> N;

    // Graine aléatoire
    int seed;
    std::cout << "Entrez la graine aléatoire : ";
    std::cin >> seed;

    // Choix de la distribution
    bool useNormalDistribution;
    std::cout << "Utiliser une distribution normale (Bernoulli sinon)?  (0/1)";
    std::cin >> useNormalDistribution;  

    // Sparse ratio
    float sparseRatio;
    std::cout << "Entrez le sparse ratio (float entre 0 et 1) : ";
    std::cin >> sparseRatio;  


    // Création d'un objet SparseMatrixGenerator
    SparseMatrixGenerator matrixGenerator(N, seed, useNormalDistribution, sparseRatio);

    // Génération de la matrice sparse
    std::vector<std::vector<double>> sparseMatrix = matrixGenerator.generateSparseMatrix();

    // Affichage de la matrice sparse générée
    std::cout << "Matrice sparse générée :\n";
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << sparseMatrix[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    return 0;
}
