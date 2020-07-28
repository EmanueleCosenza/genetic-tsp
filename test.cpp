#include <iostream>
#include <vector>
#include <algorithm>

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void add_node();
    void add_edge(int n1, int n2, float weight);
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

int main(int argc, char* argv[]) {

    int seed = atoi(argv[1]);
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
    std::vector<int> path{3,2,1,0};
    std::cout << g.is_hamiltonian(path) << '\n';
    std::vector<int> ham = g.rand_hamiltonian();
    for (auto i : ham) {
        std::cout << i << " ";
    }

    return 0;
}
