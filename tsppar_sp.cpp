#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <cstdlib>

#include "graph.hpp"
#include "genetic.hpp"

// void pop_job(TspGraph g, std::size_t pop_size, float cross_prob, float mut_prob, int max_gen, unsigned int seed) {
//     // Create initial population of tsp paths
//     TspPopulation population {pop_size, g};
//     population.init_population();
//
//     bool running = true;
//     int generations = 0;
//
//     // Genetic algorithm loop
//     while (running && generations < max_gen) {
//
//         std::cout << "Generation " << generations << '\n';
//
//         population.compute_scores();
//
//         // Print population
//         // population.print_scores();
//         // std::cout << '\n';
//
//         // Print gen info
//         std::cout << "Best path: ";
//         print_path(population.get_best_path());
//         printf("Best length: %f", population.get_best_length());
//         std::cout << '\n';
//
//         std::vector<std::vector<int>> new_individuals;
//
//         while (new_individuals.size() < pop_size) {
//             std::vector<int> pa;
//             std::vector<int> pb;
//             // Select 2 different random parents based on fitness probabilities
//             pa = population.pick_parent();
//             do {
//                 pb = population.pick_parent();
//             } while (pb == pa);
//
//             // Crossover and mutation (based on probabilities)
//             float r_cross = (float) std::rand() / (float) RAND_MAX;
//             float r_mut = (float) std::rand() / (float) RAND_MAX;
//             if (r_cross < cross_prob) {
//                 crossover(pa, pb);
//             }
//
//             if (r_mut < mut_prob) {
//                 mutate(pa);
//                 mutate(pb);
//             }
//
//             // Add the 2 new children to new population (if valid)
//             if (g.is_hamiltonian(pa) && std::find(new_individuals.begin(), new_individuals.end(), pa) == new_individuals.end()) {
//                 new_individuals.push_back(pa);
//             }
//             if (new_individuals.size() < pop_size) {
//                 if (g.is_hamiltonian(pb) && std::find(new_individuals.begin(), new_individuals.end(), pb) == new_individuals.end()) {
//                     new_individuals.push_back(pb);
//                 }
//             }
//         }
//         population.set_individuals(new_individuals);
//         generations++;
//     }
//
//     return;
// }


int main(int argc, char* argv[]) {

    if (argc != 8) {
        std::cerr << "Usage: graph_file pop_size cross_prob mut_prob max_gen nw seed\n";
        return -1;
    }

    std::string graph_file = argv[1];
    std::size_t pop_size = atoi(argv[2]);
    float cross_prob = atof(argv[3]);
    float mut_prob = atof(argv[4]);
    int max_gen = atoi(argv[5]);
    int nw = atoi(argv[6]);
    unsigned int seed = atoi(argv[7]);

    // Initialize graph
    TspGraph g;
    g.from(graph_file);
    g.print();

    std::cout << "\n";

    // -----------Thread task----------------

    auto pop_task = [&](std::size_t pop_size, unsigned int seed) {

        std::cout << "Started thread with seed " << seed << " and pop " << pop_size << '\n';

        // Create initial population of tsp paths
        TspPopulation population {pop_size, g};
        population.init_population(&seed);

        bool running = true;
        int generations = 0;

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
                if (g.is_hamiltonian(pa) && std::find(new_individuals.begin(), new_individuals.end(), pa) == new_individuals.end()) {
                    new_individuals.push_back(pa);
                }
                if (new_individuals.size() < pop_size) {
                    if (g.is_hamiltonian(pb) && std::find(new_individuals.begin(), new_individuals.end(), pb) == new_individuals.end()) {
                        new_individuals.push_back(pb);
                    }
                }
            }
            population.set_individuals(new_individuals);
            generations++;
        }
    };

    // ----------------------------------


    std::vector<std::thread> tids;
    int n_subpop = pop_size / nw;

    auto start = std::chrono::high_resolution_clock::now();

    for(int i=0; i<nw; i++) {
        tids.push_back(std::thread(pop_task, n_subpop, seed + i));
    }

    for(std::thread& t: tids) {
        t.join();
    }

    std::cout << '\n';
    auto end = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Total time: " << tot_time << " ms" << std::endl;

    return 0;
}
