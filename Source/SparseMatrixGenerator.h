#ifndef SPARSE_MATRIX_GENERATOR_H
#define SPARSE_MATRIX_GENERATOR_H

#include <vector>
#include <functional>
#include <random>

class SparseMatrixGenerator {        // The class
    public:
        
        int N;
        int M;
        int seed;
        bool useNormalDistribution;
        float sparseRatio;
        
        

        // Fonction pour générer une matrice sparse
        std::vector<std::vector<float>> generateSparseMatrix();

        // Fonction générique pour la génération de la matrice en fonction de la distribution
        void generateMatrix(std::vector<std::vector<float>>& matrix, std::function<float(std::default_random_engine&)> distributionFunction);
    
        SparseMatrixGenerator(int a, int b, int c, bool d, float e) {
            N = a;
            M = b;
            seed = c;
            useNormalDistribution = d;
            sparseRatio = e;
        }
};

#endif // SPARSE_MATRIX_GENERATOR_H