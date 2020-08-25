#include <iostream>
#include <fstream>
#include "graph.hpp"

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr << "Usage: graph_file nodes\n";
        return -1;
    }

    unsigned int seed = 1;

    std::string filename = argv[1];
    int nodes = atoi(argv[2]);

    TspGraph g;
    g.rand_complete(nodes, &seed);
    g.to(filename);
    g.print();

    return 0;
}
