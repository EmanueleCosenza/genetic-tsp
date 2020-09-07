#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "graph.hpp"

// Constructs graph from a properly formatted file
void TspGraph::from(std::string filename) {
    std::ifstream file(filename);

    int nodes;
    file >> nodes;
    for (int i=0; i<nodes; i++) {
        adj_list.push_back(std::vector<std::pair<int, float>>());
    }

    int n1, n2;
    float w;
    while (file >> n1 >> n2 >> w) {
        this->add_edge(n1, n2, w);
    }
}

// Write graph to file
void TspGraph::to(std::string filename) {
    std::ofstream file(filename);

    file << adj_list.size() << '\n';
    for (std::size_t i=0; i<adj_list.size(); i++) {
        for (auto pair : adj_list[i]) {
            file << i << " " << pair.first << " " << pair.second << '\n';
        }
    }
}

// Creates a complete graph with random weights
void TspGraph::rand_complete(int nodes, unsigned int *seedp) {
    for (int i=0; i<nodes; i++) {
        this->add_node();
    }
    for (int i=0; i<nodes; i++) {
        for (int j=i+1; j<nodes; j++) {
            if (j!=i) this->add_edge(i, j, rand_r(seedp)%9);
        }
    }
}

void TspGraph::add_node() {
    // Add a new empty adjacency list
    adj_list.push_back(std::vector<std::pair<int, float>>());
}

void TspGraph::add_edge(int n1, int n2, float weight) {
    if (this->are_linked(n1, n2)) return;
    // Add an undirected edge with the specified weight
    adj_list[n1].push_back(std::pair<int, float>(n2, weight));
    adj_list[n2].push_back(std::pair<int, float>(n1, weight));
}

bool TspGraph::are_linked(int n1, int n2) {
    for (auto pair : adj_list[n1]) {
        if (pair.first == n2) {
            return true;
        }
    }
    return false;
}

float TspGraph::path_length(std::vector<int> path) {
    float sum = 0;
    int curr_node = path[0];

    for (std::size_t i=1; i<path.size(); i++) {
        bool found = false;
        for (auto pair : adj_list[curr_node]) {
            if (pair.first == path[i]) {
                found = true;
                sum += pair.second;
                break;
            }
        }
        if (!found) return -1;
        else curr_node = path[i];
    }

    return sum;
}

// Checks if the path is an hamiltonian path of this graph
// Path must contain nodes all different from each other
bool TspGraph::is_hamiltonian(std::vector<int> path) {
    int curr_node = path[0];
    for (std::size_t i=1; i<path.size(); i++) {
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

// Returns an empty list or a random hamiltonian path of this graph
std::vector<int> TspGraph::rand_hamiltonian(unsigned int *seedp) {
    std::vector<int> path;
    int curr_node = rand_r(seedp) % adj_list.size();
    path.push_back(curr_node);

    while (path.size() < adj_list.size()) {
        std::vector<int> pickable;
        for (auto pair : adj_list[curr_node]) {
            if(std::find(path.begin(), path.end(), pair.first) == path.end()) {
                pickable.push_back(pair.first);
            }
        }
        if (!pickable.empty()) {
            curr_node = pickable[rand_r(seedp) % pickable.size()];
            path.push_back(curr_node);
        }
        else return std::vector<int>();
    }

    return path;
}

// Finds tsp optimal path and its length with a bruteforce algorithm
std::pair<float, std::vector<int>> TspGraph::bruteforce(std::vector<int> path) {

    std::vector<int> pickable;

    // Leaf of the recursion tree
    if (path.size() == adj_list.size()) return std::make_pair(0, path);
    if (path.empty()) {
        // Root of the recursion tree: first call
        for (std::size_t i=0; i<adj_list.size(); i++) {
            pickable.push_back(i);
        }
    } else {
        //
        int last_node = path[path.size()-1];
        for (auto pair : adj_list[last_node]) {
            // Iterate over nodes in the adjacency list of the last node in the path
            if(std::find(path.begin(), path.end(), pair.first) == path.end()) {
                // Add node to pickable only if it is not already in path
                pickable.push_back(pair.first);
            }
        }
    }

    if (pickable.empty())
        // Cannot explore any other node, fail
        return std::make_pair(std::numeric_limits<float>::infinity(), path);

    float min = std::numeric_limits<float>::infinity();
    std::vector<int> min_path;

    // Iterate over the pickable nodes and recursively call the function
    for (auto node : pickable) {
        std::pair<float, std::vector<int>> len_and_path;

        path.push_back(node);
        len_and_path = bruteforce(path);
        path.pop_back();

        float tot_len = len_and_path.first;
        if (!path.empty()) {
            // Add to tot_len the weight from last node to this node
            int last_node = path[path.size()-1];
            for (auto pair : adj_list[last_node]) {
                if (pair.first == node) {
                    tot_len += pair.second;
                    break;
                }
            }
        }

        // Update minimum
        if (tot_len < min) {
            min = tot_len;
            min_path = len_and_path.second;
        }
    }

    return std::make_pair(min, min_path);
}

void TspGraph::print() {
    std::cout << "Printing graph" << '\n';
    for (std::size_t i=0; i<adj_list.size(); i++) {
        std::cout << "Node " << i << ":\n";
        for (auto pair : adj_list[i]) {
            std::cout << "\tNode " << pair.first << ", ";
            std::cout << "Weight: " << pair.second << "\n";
        }
    }
}

void print_path(std::vector<int> path) {
    for (auto node : path) {
        std::cout << node << " ";
    }
    std::cout << '\n';
}
