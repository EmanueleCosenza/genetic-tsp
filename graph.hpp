#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void from(std::string filename);
    void to(std::string filename);
    void rand_complete(int nodes, unsigned int *seedp);
    void add_node();
    void add_edge(int n1, int n2, float weight);
    bool are_linked(int n1, int n2);
    float path_length(std::vector<int> path);
    bool is_hamiltonian(std::vector<int> path);
    std::vector<int> rand_hamiltonian(unsigned int *seedp);
    std::pair<float, std::vector<int>> bruteforce(std::vector<int> path);
    void print();
};

void print_path(std::vector<int> path);

#endif
