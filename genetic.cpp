#include <vector>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "graph.hpp"
#include "genetic.hpp"

void crossover(std::vector<int> &pathA, std::vector<int> &pathB) {
    // Random cross-over indices
    int i = std::rand() % (pathA.size()-1);
    int j = (i+1) + (std::rand() % (pathA.size()-(i+1)));
    // Path copies
    std::vector<int> oldA = pathA;
    std::vector<int> oldB = pathB;
    int c = i;

    // Cross-over on B
    for (auto k : oldA) {
        if (std::find(oldB.begin()+i, oldB.begin()+j+1, k) != oldB.begin()+j+1) {
            pathB[c] = k;
            c++;
            if (c > j) break;
        }
    }

    c = i;

    // Cross-over on A
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

void TspPopulation::init_population() {
    // Create 'size' tsp random paths
    std::size_t generated = 0;
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

    for (std::size_t i=0; i<individuals.size(); i++) {
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
    for (std::size_t i=0; i<scores.size(); i++) {
        scores[i] /= sum;
    }
}

void TspPopulation::print_scores() {
    std::cout << "Printing population and scores:" << '\n';
    for (std::size_t i=0; i<scores.size(); i++) {
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
    std::size_t i = 0;
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
