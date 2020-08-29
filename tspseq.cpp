#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>

#include "graph.hpp"
#include "genetic.hpp"


int main(int argc, char* argv[]) {

    if (argc != 7) {
        std::cerr << "Usage: graph_file pop_size cross_prob mut_prob max_gen seed\n";
        return -1;
    }

    std::string graph_file = argv[1];
    std::size_t pop_size = atoi(argv[2]);
    float cross_prob = atof(argv[3]);
    float mut_prob = atof(argv[4]);
    int max_gen = atoi(argv[5]);
    unsigned int seed = atoi(argv[6]);

    // Initialize graph
    TspGraph g;
    g.from(graph_file);
    g.print();

    std::cout << "\n";

    // Create initial population of tsp paths
    // TspPopulation population {pop_size, g};
    // population.init_population(&seed);
    std::vector<std::vector<int>> individuals(pop_size);
    std::vector<float> scores(pop_size);

    // Initialize random population
    // init_population(individuals);
    std::size_t generated = 0;
    while (generated < pop_size) {
        std::vector<int> path = g.rand_hamiltonian(&seed);
        if (!path.empty()) {
            // If path is not already in population, add it
            if (std::find(individuals.begin(), individuals.end(), path) == individuals.end()) {
                individuals[generated] = path;
                generated++;
            }
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    // Genetic algorithm loop
    for (int gen=0; gen<=max_gen; gen++) {

        // std::cout << "Generation " << gen << '\n';

        // Compute scores
        for (std::size_t i=0; i<pop_size; i++) {
            scores[i] = g.path_length(individuals[i]);
            // score = 1/score;
            // sum += score;
            // scores.push_back(score);
        }
    }

    std::cout << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total time: " << tot_time << " ms" << std::endl;

    return 0;
}
