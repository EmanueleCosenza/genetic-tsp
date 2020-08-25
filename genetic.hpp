#ifndef GENETIC_H
#define GENETIC_H

#include <vector>
#include "graph.hpp"

void crossover(std::vector<int> &pathA, std::vector<int> &pathB, unsigned int *seedp);

void mutate(std::vector<int> &path, unsigned int *seedp);

class TspPopulation {

    TspGraph g;
    std::vector<std::vector<int>> individuals; // List of paths
    std::vector<float> scores;
    std::vector<int> best_path;
    float best_length = std::numeric_limits<float>::infinity();
    std::size_t size;

public:
    TspPopulation(std::size_t size, TspGraph g) {
        this->size = size;
        this->g = g;
    }
    void init_population(unsigned int *seedp);
    void set_individuals(std::vector<std::vector<int>> individuals);
    void compute_scores();
    std::vector<int> pick_parent(unsigned int *seedp);
    void print_scores();
    std::vector<int> get_best_path();
    float get_best_length();
    void print();
};

#endif
