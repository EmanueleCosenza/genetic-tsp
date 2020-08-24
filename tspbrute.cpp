#include <iostream>
#include <cstdlib>
#include "graph.h"

void print_path(std::vector<int> path) {
    for (auto node : path) {
        std::cout << node << " ";
    }
    std::cout << '\n';
}

int main(int argc, char* argv[]) {

    TspGraph g;

    int nodes = atoi(argv[1]);
    int seed = atoi(argv[2]);
    std::srand(seed);

    for (int i=0; i<nodes; i++) {
        g.add_node();
    }
    for (int i=0; i<nodes; i++) {
        for (int j=i+1; j<nodes; j++) {
            if (j!=i) g.add_edge(i, j, std::rand()%9 + 1);
        }
    }
    g.print();

    std::pair<float, std::vector<int>> len_and_path = g.bruteforce(std::vector<int>());

    std::cout << len_and_path.first << '\n';
    print_path(len_and_path.second);

    return 0;
}
