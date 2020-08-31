#ifndef GENETIC_H
#define GENETIC_H

#include <vector>
#include "graph.hpp"

void crossover(std::vector<int> &pathA, std::vector<int> &pathB, unsigned int *seedp);

void mutate(std::vector<int> &path, unsigned int *seedp);

std::vector<std::vector<int>> init_population(TspGraph g, std::size_t pop_size, unsigned int *seed);

std::vector<int> pick_parent(std::vector<std::vector<int>> pop, std::vector<float> scores,
        int k, unsigned int *seedp);

#endif
