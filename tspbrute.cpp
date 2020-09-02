#include <iostream>
#include <fstream>
#include "graph.hpp"


int main(int argc, char* argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: graph_file" << std::endl;
        return -1;
    }

    std::string filename = argv[1];

    TspGraph g;
    g.from(filename);
    g.print();

    std::pair<float, std::vector<int>> len_and_path = g.bruteforce(std::vector<int>());

    std::cout << "Min path length: " << len_and_path.first << std::endl;
    std::cout << "Path:" << std::endl;
    print_path(len_and_path.second);

    return 0;
}
