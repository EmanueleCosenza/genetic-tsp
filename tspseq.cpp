#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "graph.h"

void print_path(std::vector<int> path) {
    for (auto node : path) {
        std::cout << node << " ";
    }
    std::cout << '\n';
}

void crossover(std::vector<int> &pathA, std::vector<int> &pathB) {
    int i = std::rand() % (pathA.size()-1);
    int j = (i+1) + (std::rand() % (pathA.size()-(i+1)));

    std::vector<int> oldA = pathA;
    std::vector<int> oldB = pathB;

    int c = i;

    for (auto k : oldA) {
        if (std::find(oldB.begin()+i, oldB.begin()+j+1, k) != oldB.begin()+j+1) {
            pathB[c] = k;
            c++;
            if (c > j) break;
        }
    }

    c = i;

    for (auto k : oldB) {
        if (std::find(oldA.begin()+i, oldA.begin()+j+1, k) != oldA.begin()+j+1) {
            pathA[c] = k;
            c++;
            if (c > j) break;
        }
    }

}

void mutate(std::vector<int> &path) {
    int i = std::rand() % (path.size()-1);
    int j = (i+1) + (std::rand() % (path.size()-(i+1)));

    int temp = path[i];
    path[i] = path[j];
    path[j] = temp;
}

class TspPopulation {

    TspGraph g;
    std::vector<std::vector<int>> individuals; // List of paths
    std::vector<float> scores;
    std::vector<int> best_path;
    float best_length = std::numeric_limits<float>::infinity();
    int size;

public:
    TspPopulation(int size, TspGraph g) {
        this->size = size;
        this->g = g;
    }
    void init_population();
    void set_individuals(std::vector<std::vector<int>> individuals);
    void compute_scores();
    std::vector<int> pick_parent();
    void print_scores();
    std::vector<int> get_best_path();
    float get_best_length();
    void print();
};

void TspPopulation::init_population() {
    // Create 'size' tsp random paths
    int generated = 0;
    while (generated < size) {
        std::vector<int> path = g.rand_hamiltonian();
        if (!path.empty()) {
            // If path is not already in population, add it
            if (std::find(individuals.begin(), individuals.end(), path) == individuals.end()) {
                individuals.push_back(path);
                generated++;
            }
        }
    }
}

void TspPopulation::set_individuals(std::vector<std::vector<int>> individuals) {
    this->individuals = individuals;
    size = individuals.size();
    scores = std::vector<float>();
}


// Compute scores as mating probabilities (shorter path -> higher prob.)
void TspPopulation::compute_scores() {
    float sum = 0;
    float score;

    for (int i=0; i<individuals.size(); i++) {
        score = g.path_length(individuals[i]);
        // Update best path if necessary
        if (score<best_length) {
            best_length = score;
            best_path = individuals[i];
        }
        score = 1/score;
        sum += score;
        scores.push_back(score);
    }
    for (int i=0; i<scores.size(); i++) {
        scores[i] /= sum;
    }

}

void TspPopulation::print_scores() {
    std::cout << "Printing population and scores:" << '\n';
    for (int i=0; i<scores.size(); i++) {
        for (auto node : individuals[i]) {
            std::cout << node << " ";
        }
        std::cout << ": " << scores[i] << '\n';
    }
}

std::vector<int> TspPopulation::get_best_path() {
    return best_path;
}

float TspPopulation::get_best_length() {
    return best_length;
}

// Pick a parent path according to computed probabilities
std::vector<int> TspPopulation::pick_parent() {
    float r = (float) std::rand() / (float) RAND_MAX; // random between 0 and 1
    int i = 0;
    while (r>0) {
        r -= scores[i];
        if (r>0) {
            i++;
            if (i >= individuals.size()) {
                i = individuals.size()-1;
                break;
            }
        }
    }
    return individuals[i];
}



int main(int argc, char* argv[]) {

    if (argc != 7) {
        std::cerr << "Usage: graph_file pop_size cross_prob mut_prob max_gen seed\n";
        return -1;
    }

    std::string graph_file = argv[1];
    int pop_size = atoi(argv[2]);
    float cross_prob = atof(argv[3]);
    float mut_prob = atof(argv[4]);
    int max_gen = atoi(argv[5]);
    int seed = atoi(argv[6]);

    std::srand(seed);

    // Initialize graph
    TspGraph g;
    g.from(graph_file);
    g.print();

    std::cout << "\n";

    // Create initial population of tsp paths
    TspPopulation population {pop_size, g};
    population.init_population();

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
            pa = population.pick_parent();
            do {
                pb = population.pick_parent();
            } while (pb == pa);

            // Crossover and mutation (based on probabilities)
            float r_cross = (float) std::rand() / (float) RAND_MAX;
            float r_mut = (float) std::rand() / (float) RAND_MAX;
            if (r_cross < cross_prob) {
                crossover(pa, pb);
            }

            if (r_mut < mut_prob) {
                mutate(pa);
                mutate(pb);
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

    return 0;
}
