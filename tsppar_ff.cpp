#include <iostream>
#include <ff/ff.hpp>
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>
#include "graph.hpp"
#include <chrono>
#include <thread>
using namespace ff;

enum Phase {Evaluation, Evolution};

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
    Phase phase = Evaluation;
    std::vector<std::vector<float>> sub_scores;
    int sc_rec = 0;


    Master(int nw, int max_gen, std::vector<std::vector<int>> pop): nw(nw), max_gen(max_gen), pop(pop) {
        sub_scores = std::vector<std::vector<float>>(nw);
    };

    Task* svc(Task *t) {
        if (t == NULL) {
            std::cout << "Generation " << gen << '\n';
            for(size_t i=0; i<nw; ++i) {
                ff_send_out_to(new Task(pop, std::vector<float>()), i);
            }
            return GO_ON;
        }

        switch (phase) {
            case Evaluation:
                sub_scores[get_channel_id()] = t->scores;
                std::cout << "Received scores" << '\n';

                if (++sc_rec == nw) {
                    // Received all scores from workers
                    sub_scores = std::vector<std::vector<float>>(nw);
                    sc_rec = 0;
                    gen++;
                    std::cout << "Generation " << gen << '\n';
                    delete t;
                    if (gen >= max_gen) return EOS;
                    for(size_t i=0; i<nw; ++i) {
                        ff_send_out_to(new Task(pop, std::vector<float>()), i);
                    }
                    return GO_ON;
                }
                break;
            case Evolution:
                break;
            default:
                break;
        }

        delete t;

        return GO_ON;
    }

};

struct Worker: ff_node_t<Task> {

    TspGraph g;
    std::size_t pop_size;
    float cross_prob;
    float mut_prob;
    std::pair<std::size_t, std::size_t> range;

    Worker(TspGraph g, std::size_t pop_size, float cross_prob, float mut_prob, std::pair<std::size_t, std::size_t> range):
        g(g), pop_size(pop_size), cross_prob(cross_prob), mut_prob(mut_prob), range(range) {};

    Task* svc(Task *t) {
        std::vector<std::vector<int>> pop = t->pop;
        std::vector<float> scores(range.second-range.first+1);

        for (int i=0; i<range.second-range.first+1; i++) {
            scores[i] = g.path_length(pop[i]);
        }

        delete t;

        return new Task(pop, scores);
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
    for (int i = 0; i < nw; ++i)
        w.push_back(std::make_unique<Worker>(g, pop_size, cross_prob, mut_prob, ranges[i]));
    std::vector<std::vector<int>> pop_i = init_population(g, pop_size, &seed);
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
    std::cout << "Time: " << ffTime(GET_TIME) << "\n";

    return 0;
}
