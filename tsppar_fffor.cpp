#include <iostream>
#include <chrono>
#include <thread>

#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>
#include "graph.hpp"
#include "genetic.hpp"

using namespace ff;


int main(int argc, char *argv[]) {

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

<<<<<<< HEAD
=======
    ParallelFor pf(nw);
>>>>>>> 69364db9fdaa9bf45f69c2a5187e58545d3ef61f
    std::vector<std::vector<int>> pop = init_population(g, pop_size, &seed);
    std::vector<std::vector<int>> new_pop(pop_size);
    std::vector<float> scores(pop_size);

<<<<<<< HEAD
    ParallelFor pf(nw);

=======
>>>>>>> 69364db9fdaa9bf45f69c2a5187e58545d3ef61f
    ffTime(START_TIME);
    for (int gen=0; gen<max_gen; gen++) {

        // Compute fitness scores
<<<<<<< HEAD
        pf.parallel_for(0, pop_size, 1, 0, [&](const long i) {
            scores[i] = g.path_length(pop[i]);
        }, nw);

        pf.parallel_for(0, pop_size, 1, 0, [&](const long i) {
=======
        pf.parallel_for(0, pop_size, [&](const long i) {
            scores[i] = g.path_length(pop[i]);
        }, nw);

        pf.parallel_for(0, pop_size, [&](const long i) {
>>>>>>> 69364db9fdaa9bf45f69c2a5187e58545d3ef61f
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

                if (g.is_hamiltonian(pa)) {
                    new_pop[i] = pa;
                    added = true;
                }
                else if (g.is_hamiltonian(pb)) {
                    new_pop[i] = pb;
                    added = true;
                }
            }
        }, nw);
        std::swap(pop, new_pop);
    }
    ffTime(STOP_TIME);

    auto min_el = std::min_element(scores.begin(), scores.end());
    int min_pos = std::distance(scores.begin(), min_el);

    std::cout << "Min path length: " << scores[min_pos] << std::endl;
    std::cout << "Path: " << std::endl;
    print_path(pop[min_pos]);

    std::cout << "Total time: " << ffTime(GET_TIME) << "\n";

    return 0;
}
