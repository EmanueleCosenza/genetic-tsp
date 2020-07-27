#include <iostream>
#include <vector>

class TspGraph {

    std::vector<std::vector<std::pair<int, float>>> adj_list;

public:
    void add_node();
    void add_edge(int n1, int n2, float weight);
    bool is_tsp_path(std::vector<int> path);
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
        bool found = false;
        for (auto pair : adj_list[prec]) {
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

int main(int argc, char* argv[]) {

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
    std::cout << g.is_tsp_path(path) << '\n';

    return 0;
}
