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
    TspPopulation population {pop_size, g};
    population.init_population(&seed);

    bool running = true;
    int generations = 0;

    auto start = std::chrono::high_resolution_clock::now();

    // Genetic algorithm loop
    while (running && generations < max_gen) {

        std::cout << "Generation " << generations << '\n';

        population.compute_scores();

        // Print population
        // population.print_scores();
        // std::cout << '\n';

        // Print gen info
        std::cout << "Best path: ";
        print_path(population.get_best_path());
        printf("Best length: %f", population.get_best_length());
        std::cout << '\n';

        std::vector<std::vector<int>> new_individuals;

        while (new_individuals.size() < pop_size) {
            std::vector<int> pa;
            std::vector<int> pb;
            // Select 2 different random parents based on fitness probabilities
            pa = population.pick_parent(&seed);
            do {
                pb = population.pick_parent(&seed);
            } while (pb == pa);

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

            // Add the 2 new children to new population (if valid)
            if (g.is_hamiltonian(pa)) {
                new_individuals.push_back(pa);
            }
            if (new_individuals.size() < pop_size) {
                if (g.is_hamiltonian(pb)) {
                    new_individuals.push_back(pb);
                }
            }
        }
        population.set_individuals(new_individuals);
        generations++;
    }

    std::cout << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total time: " << tot_time << " ms" << std::endl;

    return 0;
}
