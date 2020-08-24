#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>

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

    // std::cout << "Indices: " << i << " " << j << '\n';

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

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void add_node();
    void add_edge(int n1, int n2, float weight);
    float path_length(std::vector<int> path);
    bool is_hamiltonian(std::vector<int> path);
    std::vector<int> rand_hamiltonian();
    void print();
};

void TspGraph::add_node() {
    // Add new empty adjacency list
    adj_list.push_back(std::vector<std::pair<int, float>>());
}

void TspGraph::add_edge(int n1, int n2, float weight) {
    // Add undirected edge with the specified weight
    adj_list[n1].push_back(std::pair<int, float>(n2, weight));
    adj_list[n2].push_back(std::pair<int, float>(n1, weight));
}

float TspGraph::path_length(std::vector<int> path) {
    float sum = 0;
    int curr_node = path[0];

    for (int i=1; i<path.size(); i++) {
        bool found = false;
        for (auto pair : adj_list[curr_node]) {
            if (pair.first == path[i]) {
                found = true;
                sum += pair.second;
                break;
            }
        }
        if (!found) return -1;
        else curr_node = path[i];
    }

    return sum;
}

// Checks if the path is an hamiltonian path of this graph
bool TspGraph::is_hamiltonian(std::vector<int> path) {
    // path contains V different nodes
    int curr_node = path[0];
    for (int i=1; i<path.size(); i++) {
        bool found = false;
        for (auto pair : adj_list[curr_node]) {
            if (pair.first == path[i]) {
                found = true;
                break;
            }
        }
        if (!found) return false;
        else curr_node = path[i];
    }
    return true;
}

// Returns an empty list or a random hamiltonian path of this graph
std::vector<int> TspGraph::rand_hamiltonian() {
    std::vector<int> path;
    int curr_node = std::rand() % adj_list.size();
    path.push_back(curr_node);

    while (path.size() < adj_list.size()) {
        std::vector<int> pickable;
        for (auto pair : adj_list[curr_node]) {
            if(std::find(path.begin(), path.end(), pair.first) == path.end()) {
                pickable.push_back(pair.first);
            }
        }
        if (!pickable.empty()) {
            curr_node = pickable[std::rand() % pickable.size()];
            path.push_back(curr_node);
        }
        else return std::vector<int>();
    }

    return path;
}

void TspGraph::print() {
    std::cout << "Printing graph" << '\n';
    for (int i=0; i<adj_list.size(); i++) {
        std::cout << "Node " << i << ":\n";
        for (auto pair : adj_list[i]) {
            std::cout << "\tNode " << pair.first << ", ";
            std::cout << "Weight: " << pair.second << "\n";
        }
    }
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

    if (argc != 5) {
        std::cerr << "Usage: pop_size cross_prob mut_prob seed\n";
        return -1;
    }

    int pop_size = atoi(argv[1]);
    float cross_prob = atof(argv[2]);
    float mut_prob = atof(argv[3]);
    int seed = atoi(argv[4]);

    std::srand(seed);

    // Initialize graph
    TspGraph g;

    int ns = 20;
    for (int i=0; i<ns; i++) {
        g.add_node();
    }
    for (int i=0; i<ns; i++) {
        for (int j=i+1; j<ns; j++) {
            if (j!=i) g.add_edge(i, j, j);
        }
    }
    g.print();

    std::cout << "\n";

    // Create initial population of tsp paths
    TspPopulation population {pop_size, g};
    population.init_population();

    bool running = true;
    int generations = 0;

    // Genetic algorithm loop
    while (running && generations < 10000) {

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

            // std::cout << "Selected parents:" << '\n';
            // print_path(pa);
            // print_path(pb);

            // Crossover AND mutate (based on probabilities)
            float r_cross = (float) std::rand() / (float) RAND_MAX;
            float r_mut = (float) std::rand() / (float) RAND_MAX;
            if (r_cross < cross_prob) {
                crossover(pa, pb);
                // std::cout << "After crossover:" << '\n';
                // print_path(pa);
                // print_path(pb);
            }

            if (r_mut < mut_prob) {
                mutate(pa);
                mutate(pb);
                // std::cout << "After mutation:" << '\n';
                // print_path(pa);
                // print_path(pb);
            }

            // Add the 2 new children to new population (if valid)
            if (g.is_hamiltonian(pa) && std::find(new_individuals.begin(), new_individuals.end(), pa) == new_individuals.end()) {
                new_individuals.push_back(pa);
                // std::cout << "Valid A" << '\n';
            }
            if (new_individuals.size() < pop_size) {
                if (g.is_hamiltonian(pb) && std::find(new_individuals.begin(), new_individuals.end(), pb) == new_individuals.end()) {
                    new_individuals.push_back(pb);
                    // std::cout << "Valid B" << '\n';
                }
            }
            // std::cout << "New population size: " << new_individuals.size() << '\n';
            // std::cout << '\n';
        }
        // population = TspPopulation(new_individuals);
        population.set_individuals(new_individuals);
        generations++;
    }

    return 0;
}
