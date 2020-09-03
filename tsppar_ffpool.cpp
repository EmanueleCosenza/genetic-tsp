#include <iostream>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include <ff/poolEvolution.hpp>
#include "graph.hpp"
#include "genetic.hpp"
using namespace ff;

typedef struct Env_t {
    int max_gen;
    int curr_gen;
    TspGraph g;
    std::vector<float> scores;
    std::size_t pop_size;
    float cross_prob;
    float mut_prob;
    unsigned int seed;
} Env_t;

bool termination(const std::vector<std::vector<int>> &P, Env_t &env) {
    return env.curr_gen >= env.max_gen;
}

void selection(ParallelForReduce<std::vector<int>> &mapreduce, std::vector<std::vector<int>> &pop,
    std::vector<std::vector<int>> &new_pop, Env_t &env) {

    env.curr_gen++;
    mapreduce.parallel_for(0, pop.size(), [&](const long i) {
        env.scores[i] = (env.g).path_length(pop[i]);
    });
    // for(int i=0; i<pop.size(); i++) {
    //     print_path(pop[i]);
    //     std::cout << "Score: " << env.scores[i] << '\n';
    // }
    // std::cout << "--------------------" << '\n';

    new_pop = pop;

}

const std::vector<int>& evolution(std::vector<int> &ind, const Env_t &env, int thid) {
    return ind;
}

void filter(ParallelForReduce<std::vector<int>> &mapreduce, std::vector<std::vector<int>> &pop,
    std::vector<std::vector<int>> &new_pop, Env_t &env) {

    mapreduce.parallel_for(0, pop.size(), [&](const long i) {
        bool added = false;

        while (!added) {
            // Select 2 different random parents based on fitness scores
            // TODO: CHANGE K
            std::vector<int> pa = pick_parent(pop, env.scores, 10, &env.seed);
            std::vector<int> pb = pick_parent(pop, env.scores, 10, &env.seed);

            // Crossover and mutation (based on probabilities)
            float r_cross = (float) rand_r(&env.seed) / (float) RAND_MAX;
            float r_mut = (float) rand_r(&env.seed) / (float) RAND_MAX;
            if (r_cross < env.cross_prob) {
                crossover(pa, pb, &env.seed);
            }
            if (r_mut < env.mut_prob) {
                mutate(pa, &env.seed);
                mutate(pb, &env.seed);
            }

            if (env.g.is_hamiltonian(pa)) {
                new_pop[i] = pa;
                added = true;
            }
            else if (env.g.is_hamiltonian(pb)) {
                new_pop[i] = pb;
                added = true;
            }
        }
    });

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
    std::vector<float> scores = std::vector<float>(pop_size);
    Env_t env;
    env.max_gen = max_gen;
    env.curr_gen = 0;
    env.g = g;
    env.scores = scores;
    env.pop_size = pop_size;
    env.cross_prob = cross_prob;
    env.mut_prob = mut_prob;
    env.seed = seed;

    // Initialize population
    std::vector<std::vector<int>> pop = init_population(g, pop_size, &seed);

    // Initialize pool and start
    poolEvolution<std::vector<int>, Env_t> pool(nw, pop, selection, evolution, filter, termination, env);

    ffTime(START_TIME);
    if (pool.run_and_wait_end() < 0) {
        std::cerr << "Pool failed to run.\n";
        return -1;
    }
    ffTime(STOP_TIME);

    pop = pool.get_result();
    env = pool.getEnv();

    auto min_el = std::min_element(env.scores.begin(), env.scores.end());
    int min_pos = std::distance(env.scores.begin(), min_el);

    std::cout << "Min path length: " << env.scores[min_pos] << std::endl;
    std::cout << "Path: " << std::endl;
    print_path(pop[min_pos]);

    std::cout << "Total time: " << ffTime(GET_TIME) << "\n";

    return 0;
}
