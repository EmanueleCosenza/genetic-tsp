#include <iostream>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
using namespace ff;

typedef struct Env_t {
    int max_gen;
    int curr_gen;
    TspGraph g;
    std::vector<float> scores;
    std::size_t pop_size;
    float cross_prob;
    float mut_prob;
} Env_t;

bool termination(const std::vector<std::vector<int>> &P, Env_t &env) {
    return env.curr_gen >= max_gen;
}

void selection(ParallelForReduce<std::vector<int>> &mapreduce, std::vector<std::vector<int>> &pop,
    std::vector<std::vector<int>> &new_pop, Env_t &env) {

    env.curr_gen++;
    mapreduce->parallel_for(0, pop.size(), [&](const long i) {
        env->scores[i] = (env->g).path_length(pop[i]);
    });

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

    // Initialize environment struct
    Env_t env;
    env.max_gen = max_gen;
    env.curr_gen = 0;
    env.g = g;
    env.pop_size = pop_size;
    env.cross_prob = cross_prob;
    env.mut_prob = mut_prob;

    // Initialize population
    std::vector<std:vector<int>> pop = init_population(g, pop_size, &seed);

    // Initialize pool and start
    poolEvolution<std::vector<int>, Env_t> pool(nw, pop, selection, evolution, filter, termination, env);
    if (pool.run_and_wait_end() < 0) {
        std::cerr << "Pool failed to run.\n";
        return -1;
    }

    return 0;
}
