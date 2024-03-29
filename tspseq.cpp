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

    std::cout << std::endl;

    // Create initial population of tsp paths
    std::vector<std::vector<int>> pop = init_population(g, pop_size, &seed);
    std::vector<float> scores(pop_size);

    auto start = std::chrono::high_resolution_clock::now();

    // Genetic algorithm loop
    for (int gen=0; gen<=max_gen; gen++) {

        // Compute scores
        for (std::size_t i=0; i<pop_size; i++) {
            scores[i] = g.path_length(pop[i]);
        }

        std::vector<std::vector<int>> new_pop(pop_size);

        for (std::size_t i=0; i<new_pop.size(); i++) {
            bool added = false;
            while (!added) {
                // Select 2 different random parents based on fitness scores
                // TODO: CHANGE K
                std::vector<int> pa = pick_parent(pop, scores, 10, &seed);
                std::vector<int> pb = pick_parent(pop, scores, 10, &seed);

                // Crossover and mutation (based on probabilities)
                float r_cross = (float) rand_r(&seed) / (float) RAND_MAX;
                float r_mut = (float) rand_r(&seed) / (float) RAND_MAX;
                if (r_cross < cross_prob) {
                    crossover(pa, pb, &seed);
                }
                if (r_mut < mut_prob) {
                    mutate(pa, &seed);
                    mutate(pb, &seed);
                }

                // Add the 2 new paths to new population (if hamiltonian)
                if (g.is_hamiltonian(pa)) {
                    new_pop[i] = pa;
                    added = true;
                }
                else if (g.is_hamiltonian(pb)) {
                    new_pop[i] = pb;
                    added = true;
                }
            }
        }
        pop = new_pop;
    }

    // Compute minimum and print
    auto min_el = std::min_element(scores.begin(), scores.end());
    int min_pos = std::distance(scores.begin(), min_el);
    std::cout << "Min path length: " << scores[min_pos] << std::endl;
    std::cout << "Path: " << std::endl;
    print_path(pop[min_pos]);

    auto end = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << std::endl;
    std::cout << "Total time: " << tot_time << " ms" << std::endl;

    return 0;
}
