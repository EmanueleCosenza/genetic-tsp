#include <iostream>
#include <chrono>
#include <thread>

#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include "graph.hpp"
#include "genetic.hpp"
using namespace ff;


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
    // int i = rand_r(seedp) % (pop.size()-k+1);
    // int j = i+k-1;

    float min = std::numeric_limits<float>::infinity();
    int r, ind=0;

    for (int i=0; i<k; i++) {
        r = rand_r(seedp) % (pop.size());
        if (scores[r] < min) {
            min = scores[r];
            ind = r;
        }
    }
    return pop[ind];
}

struct Task {
    std::vector<std::vector<int>> pop;
    std::vector<float> scores;

    Task(std::vector<std::vector<int>> pop, std::vector<float> scores): pop(pop), scores(scores) {};
};

struct Master: ff_monode_t<Task> {

    int nw;
    int gen = 0;
    int max_gen;
    std::vector<std::vector<int>> pop;
    std::vector<float> scores;
    std::vector<std::vector<std::vector<int>>> sub_pops;
    std::vector<std::vector<float>> sub_scores;
    int rec = 0;

    Master(int nw, int max_gen, std::vector<std::vector<int>> pop): nw(nw), max_gen(max_gen), pop(pop) {
        sub_scores = std::vector<std::vector<float>>(nw);
        sub_pops = std::vector<std::vector<std::vector<int>>>(nw);
    };

    std::vector<std::vector<int>> get_pop() {
        return pop;
    }

    std::vector<float> get_scores() {
        return scores;
    }

    Task* svc(Task *t) {
        if (t == NULL) {
            // First time
            for(size_t i=0; i<nw; ++i) {
                ff_send_out_to(new Task(pop, std::vector<float>()), i);
            }
            return GO_ON;
        }

        if ((t->pop).empty()) {
            // Evaluation feedback
            sub_scores[get_channel_id()] = t->scores;

            if (++rec == nw) {
                scores = std::vector<float>();
                for(const auto &v: sub_scores)
                    scores.insert(scores.end(), v.begin(), v.end());
                rec = 0;
                sub_scores = std::vector<std::vector<float>>(nw);
                for(size_t i=0; i<nw; ++i) {
                    ff_send_out_to(new Task(pop, scores), i);
                }
            }
            return GO_ON;
        } else {
            // Evolution feedback
            sub_pops[get_channel_id()] = t->pop;
            if (++rec == nw) {
                pop = std::vector<std::vector<int>>();
                for(const auto &v: sub_pops)
                    pop.insert(pop.end(), v.begin(), v.end());
                sub_pops = std::vector<std::vector<std::vector<int>>>(nw);
                rec = 0;
                gen++;
                if (gen >= max_gen) return EOS;
                for(size_t i=0; i<nw; ++i) {
                    ff_send_out_to(new Task(pop, std::vector<float>()), i);
                }
            }
            return GO_ON;
        }

    }

};

struct Worker: ff_node_t<Task> {

    TspGraph g;
    std::size_t pop_size;
    float cross_prob;
    float mut_prob;
    std::pair<std::size_t, std::size_t> range;
    unsigned int seed;

    Worker(TspGraph g, std::size_t pop_size, float cross_prob,
        float mut_prob, std::pair<std::size_t, std::size_t> range, unsigned int seed):
        g(g), pop_size(pop_size), cross_prob(cross_prob), mut_prob(mut_prob), range(range), seed(seed) {};

    Task* svc(Task *t) {

        if ((t->scores).empty()) {
            // Evaluation phase
            std::vector<std::vector<int>> pop = t->pop;
            std::vector<float> scores(range.second-range.first+1);

            for (int i=0; i<range.second-range.first+1; i++) {
                scores[i] = g.path_length(pop[range.first+i]);
            }

            // for (auto a : scores)
            //     std::cout << a << " ";
            // std::cout << '\n';
            // delete t;
            return new Task(std::vector<std::vector<int>>(), scores);
        } else {
            // Evolution phase
            std::vector<std::vector<int>> pop = t->pop;
            std::vector<float> scores = t->scores;

            int generated = 0, sub_pop_size = range.second-range.first+1;
            std::vector<std::vector<int>> new_sub_pop(sub_pop_size);

            while (generated < sub_pop_size) {

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
                    new_sub_pop[generated] = pa;
                    generated++;
                }
                if (generated < sub_pop_size) {
                    if (g.is_hamiltonian(pb)) {
                        new_sub_pop[generated] = pb;
                        generated++;
                    }
                }
            }

            return new Task(new_sub_pop, std::vector<float>());
        }

    }

};


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

    // Calculate partitions over population
    std::vector<std::pair<std::size_t, std::size_t>> ranges(nw);
    std::size_t delta {pop_size/nw};
    std::size_t start, end;
    for (int i=0; i<nw; i++) {
        start = (std::size_t) i*delta;
        end = (std::size_t)(i != (nw-1) ? (i+1)*delta-1 : pop_size-1);
        ranges[i] = std::make_pair(start, end);
    }

    // Build master and workers
    std::vector<std::unique_ptr<ff_node>> w;
    std::vector<std::vector<int>> pop_i = init_population(g, pop_size, &seed);
    for (int i = 0; i < nw; ++i)
        w.push_back(std::make_unique<Worker>(g, pop_size, cross_prob, mut_prob, ranges[i], seed+i));
    Master master(nw, max_gen, pop_i);

    // Build farm
    ff_Farm<Task> farm(std::move(w));
    farm.add_emitter(master);
    farm.remove_collector();
    farm.wrap_around();

    // Run farm
    ffTime(START_TIME);
    if (farm.run_and_wait_end() < 0) {
        std::cerr << "Could not run farm.\n";
        return -1;
    }
    ffTime(STOP_TIME);

    std::vector<std::vector<int>> pop = master.get_pop();
    std::vector<float> scores = master.get_scores();

    auto min_el = std::min_element(scores.begin(), scores.end());
    int min_pos = std::distance(scores.begin(), min_el);

    std::cout << "Min path length: " << scores[min_pos] << std::endl;
    std::cout << "Path: " << std::endl;
    print_path(pop[min_pos]);

    std::cout << "Time: " << ffTime(GET_TIME) << "\n";

    return 0;
}
