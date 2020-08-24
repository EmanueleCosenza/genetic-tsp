#include <iostream>
#include <fstream>
#include "graph.h"

int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: graph_file\n";
        return -1;
    }

    std::string filename = argv[1];

    TspGraph g;
    g.rand_complete(10);
    g.to(filename);
    g.print();

    // TspGraph g;
    // g.from(filename);
    // g.print();
    //
    // // g.rand_complete(10);
    // // g.print();
    //
    // std::pair<float, std::vector<int>> len_and_path = g.bruteforce(std::vector<int>());
    //
    // std::cout << len_and_path.first << '\n';
    // print_path(len_and_path.second);

    return 0;
}
