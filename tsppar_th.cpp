#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <thread>

#include "graph.hpp"
#include "genetic.hpp"
#include "sync.hpp"


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
    std::cout << std::endl;

    // Compute partitions over the population for each thread
    std::vector<std::pair<std::size_t, std::size_t>> ranges(nw);
    std::size_t delta {pop_size/nw};
    std::size_t start, end;
    for (int i=0; i<nw; i++) {
        start = (std::size_t) i*delta;
        end = (std::size_t)(i != (nw-1) ? (i+1)*delta-1 : pop_size-1);
        ranges[i] = std::make_pair(start, end);
    }

    // Initialize random population and genetic algo structures
    std::vector<std::vector<int>> pop = init_population(g, pop_size, &seed);
    std::vector<std::vector<int>> new_pop(pop_size);
    std::vector<float> scores(pop_size);

#ifdef BLOCKING
    std::vector<BlockingBarrier> barriers1(max_gen);
    std::vector<BlockingBarrier> barriers2(max_gen);
#else
    std::vector<ActiveBarrier> barriers1(max_gen);
    std::vector<ActiveBarrier> barriers2(max_gen);
#endif

    for (auto &b : barriers1)
        b.set_t(nw);
    for (auto &b : barriers2)
        b.set_t(nw);

    auto th_task = [&](std::pair<std::size_t,std::size_t> range, unsigned int seed) {

        std::size_t start = range.first;
        std::size_t end = range.second;
        std::vector<std::vector<int>> *ppop = &pop;
        std::vector<std::vector<int>> *pnew_pop = &new_pop;

        // Start evolution
        for (int gen=0; gen<max_gen; gen++) {

            // Compute fitness scores in this thread's range
            for (std::size_t i=start; i<=end; i++) {
                scores[i] = g.path_length((*ppop)[i]);
            }

            barriers1[gen].bwait();

            int generated = 0, sub_pop = end-start+1;

            while (generated < sub_pop) {

                // Select 2 different random parents based on fitness scores
                // TODO: CHANGE K
                std::vector<int> pa = pick_parent(*ppop, scores, 10, &seed);
                std::vector<int> pb = pick_parent(*ppop, scores, 10, &seed);

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
                    (*pnew_pop)[start+generated] = pa;
                    generated++;
                }
                if (generated < sub_pop) {
                    if (g.is_hamiltonian(pb)) {
                        (*pnew_pop)[start+generated] = pb;
                        generated++;
                    }
                }

            }

            barriers2[gen].bwait();

            // Swap pointers
            std::swap(ppop, pnew_pop);
        }
    };

    std::vector<std::thread> tids;
    auto start_t = std::chrono::high_resolution_clock::now();

    // Start threads and join
    for (int i=0; i<nw; i++) {
        tids.push_back(std::thread(th_task, ranges[i], seed+i));
    }
    for (std::thread &t: tids) {
        t.join();
    }

    auto end_t = std::chrono::high_resolution_clock::now();
    auto tot_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t).count();

    // Compute minimum and print
    auto min_el = std::min_element(scores.begin(), scores.end());
    int min_pos = std::distance(scores.begin(), min_el);
    std::cout << "Min path length: " << scores[min_pos] << std::endl;
    std::cout << "Path: " << std::endl;
    print_path(pop[min_pos]);

    // Print elapsed time
    std::cout << std::endl;
    std::cout << "Total time: " << tot_time << " ms" << std::endl;

    return 0;
}
