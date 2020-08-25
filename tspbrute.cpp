#include <iostream>
#include <fstream>
#include "graph.hpp"


int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: graph_file\n";
        return -1;
    }

    std::string filename = argv[1];

    TspGraph g;
    g.from(filename);
    g.print();

    std::pair<float, std::vector<int>> len_and_path = g.bruteforce(std::vector<int>());

    std::cout << len_and_path.first << '\n';
    print_path(len_and_path.second);

    return 0;
}
