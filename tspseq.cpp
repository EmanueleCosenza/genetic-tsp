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

bool TspGraph::is_tsp_path(std::vector<int> path) {
    // path contains V elements which are all different
    int prec = path[0];
    for (int i=1; i<path.size(); i++) {
        adjacent = adj_list[prec];
        found = false;
        for (auto pair : adjacent) {
            if (pair.first == path[i]) {
                found = true;
                break;
            }
        }
        if (!found) return false;
        else prec = path[i];
    }
    return true;
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

class TspPath {

    std::vector<int> path;

public:

}

class TspPopulation {

    std::vector<TspPath> individuals;
    int size;

public:
    TspPopulation(int size) {
        this->size = size;
    }
    TspPopulation(std::vector<TspPath> individuals) {
        this->individuals = individuals;
        this->size =
    }
    void init_population();
    int* compute_fitness();
};

void TSPPopulation::init_population() {

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

    }

    std::cout << "Porco dio" << '\n';

    return 0;
}
