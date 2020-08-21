#include <iostream>
#include <vector>
#include <algorithm>

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
    int curr_node = rand() % adj_list.size();
    path.push_back(curr_node);

    while (path.size() < adj_list.size()) {
        std::vector<int> pickable;
        for (auto pair : adj_list[curr_node]) {
            if(std::find(path.begin(), path.end(), pair.first) == path.end()) {
                pickable.push_back(pair.first);
            }
        }
        if (!pickable.empty()) {
            curr_node = pickable[rand() % pickable.size()];
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
    int size;

public:
    TspPopulation(int size, TspGraph g) {
        this->size = size;
        this->g = g;
    }
    // TspPopulation(std::vector<std::vector<int>> individuals) {
    //     this->individuals = individuals;
    //     this->size =
    // }
    void init_population();
    void compute_scores();
    void print_scores();
    void print();
};

void TspPopulation::init_population() {
    // Create 'size' tsp paths
    int generated = 0;
    while (generated < size) {
        std::vector<int> path = g.rand_hamiltonian();
        if (!path.empty()) {
            if (std::find(individuals.begin(), individuals.end(), path) == individuals.end()) {
                individuals.push_back(path);
                generated++;
            }
        }
    }
}

void TspPopulation::compute_scores() {
    float sum = 0;
    float score;

    for (int i=0; i<individuals.size(); i++) {
        score = g.path_length(individuals[i]);
        sum += score;
        scores.push_back(score);
    }
    for (int i=0; i<scores.size(); i++) {
        scores[i] /= sum;
    }

}

void TspPopulation::print_scores() {
    for (int i=0; i<scores.size(); i++) {
        for (auto node : individuals[i]) {
            std::cout << node << " ";
        }
        std::cout << ": " << scores[i] << '\n';
    }
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

    TspGraph g;

    g.add_node();
    g.add_node();
    g.add_node();
    g.add_node();
    g.add_edge(0, 1, 3.4);
    g.add_edge(0, 3, 2);
    g.add_edge(1, 2, 0.05);
    g.add_edge(0, 2, 0.05);
    g.add_edge(2, 3, 0.05);
    g.print();

    // Create initial population of tsp paths
    TspPopulation population {pop_size, g};
    population.init_population();

    bool running = true;

    // while (running) {
    //     population.compute_scores();
    //     // Transform scores into probabilities (normalize)
    //     // while new population hasn't reached size
    //         // Select 2 random parents based on fitness probabilities
    //         // Crossover and mutate (based on probabilities)
    //         // Add the 2 new children to new population
    // }

    population.compute_scores();
    population.print_scores();

    std::cout << "Porco dio" << '\n';

    return 0;
}
