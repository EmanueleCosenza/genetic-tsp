#include <vector>
#include <algorithm>
#include <iostream>
#include "graph.hpp"
#include "genetic.hpp"

void crossover(std::vector<int> &pathA, std::vector<int> &pathB, unsigned int *seedp) {
    // Random cross-over indices
    int i = rand_r(seedp) % (pathA.size()-1);
    int j = (i+1) + (rand_r(seedp) % (pathA.size()-(i+1)));
    // Path copies
    std::vector<int> oldA = pathA;
    std::vector<int> oldB = pathB;
    int c = i;

    // Cross-over on B
    for (auto k : oldA) {
        if (std::find(oldB.begin()+i, oldB.begin()+j+1, k) != oldB.begin()+j+1) {
            pathB[c] = k;
            c++;
            if (c > j) break;
        }
    }

    c = i;

    // Cross-over on A
    for (auto k : oldB) {
        if (std::find(oldA.begin()+i, oldA.begin()+j+1, k) != oldA.begin()+j+1) {
            pathA[c] = k;
            c++;
            if (c > j) break;
        }
    }

}

void mutate(std::vector<int> &path, unsigned int *seedp) {
    // Random mutation indices
    int i = rand_r(seedp) % (path.size()-1);
    int j = (i+1) + (rand_r(seedp) % (path.size()-(i+1)));

    // Swap
    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}

std::vector<std::vector<int>> init_population(TspGraph g, std::size_t pop_size, unsigned int *seed) {
    std::size_t generated = 0;
    std::vector<std::vector<int>> individuals(pop_size);
    while (generated < pop_size) {
        std::vector<int> path = g.rand_hamiltonian(seed);
        if (!path.empty()) {
            // If path is not already in population, add it
            if (std::find(individuals.begin(), individuals.end(), path) == individuals.end()) {
                individuals[generated] = path;
                generated++;
            }
        }
    }
    return individuals;
}

std::vector<int> pick_parent(std::vector<std::vector<int>> pop, std::vector<float> scores,
        int k, unsigned int *seedp) {

    float min = std::numeric_limits<float>::infinity();
    int r, ind=0;

    // Tournament selection: choose k random elements and pick the one with
    // minimum fitness score
    for (int i=0; i<k; i++) {
        r = rand_r(seedp) % (pop.size());
        if (scores[r] < min) {
            min = scores[r];
            ind = r;
        }
    }
    return pop[ind];
}
