#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <algorithm>
#include <cstdlib>

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void add_node();
    void add_edge(int n1, int n2, float weight);
    float path_length(std::vector<int> path);
    bool is_hamiltonian(std::vector<int> path);
    std::vector<int> rand_hamiltonian();
    std::pair<float, std::vector<int>> bruteforce(std::vector<int> path);
    void print();
};

#endif
