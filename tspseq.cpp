#include <iostream>
#include <vector>

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void add_node();
    void add_edge(int n1, int n2, float weight);
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

bool TspGraph::is_hamiltonian(std::vector<int> path) {
    // path contains V elements which are all different
    int curr_node = path[0];
    for (int i=1; i<path.size(); i++) {
        found = false;
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
            path.push_back(curr_node);compute_fitness();
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
    std::vector<std::vector<int>> individuals;
    std::vector<float> scores;
    int size;

public:
    TspPopulation(int size, TspGraph g) {
        this->size = size;
        this->g = g;
    }
    TspPopulation(std::vector<std::vector<int>> individuals) {
        this->individuals = individuals;
        this->size =
    }
    void init_population();
    int* compute_fitness();
    void print();
};

void TspPopulation::init_population() {
    // Create 'size' tsp paths
    int generated = 0;
    while (created < size) {
        std::vector<int> path = g.rand_hamiltonian();
        if (!path.empty()) {
            individuals.push_back(path);
            generated++;
        }
    }
}

int main(int argc, char* argv[]) {

    if (argc != 5) {
        fprintf(stderr, "Usage: pop_size cross_prob mut_prob seed\n");
        return -1;
    }

    int pop_size = atoi(argv[1]);
    int cross_prob = atoi(argv[2]);
    int mut_prob = atoi(argv[3]);
    int seed = atoi(argv[4]);

    std::srand(seed);

    // Create initial population of tsp paths
    TspPopulation population {pop_size};
    population.init_population();

    int* scores;
    bool running = false;

    while (running) {
        scores = population.compute_fitness();
        // Transform scores into probabilities (normalize)
        // while new population hasn't reached size
            // Select 2 random parents based on fitness probabilities
            // Crossover and mutate (based on probabilities)
            // Add the 2 new children to new population
    }

    std::cout << "Porco dio" << '\n';

    return 0;
}
