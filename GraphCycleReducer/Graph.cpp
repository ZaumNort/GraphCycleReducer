#include "Graph.h"

Graph::Graph(int V) {
    this->vertices = V;
    adjacency_list.resize(V);
    types = {};
}

const std::vector<char>& Graph::get_types() const {
        return types;
}

void Graph::AddEdge(int v, int w) {
    adjacency_list[v].push_back(w);
}

const std::vector<std::vector<int>>& Graph::get_adjacency_list() const {
    return adjacency_list;
}

int Graph::get_vertices() const {
    return vertices;
}

bool Graph::IsSubPath(std::vector<int> first, std::vector<int> second) {
    if (first.empty() || second.empty()) return false;
    if (first.size() > second.size())  return false;

    for (int i = 0; i <= second.size() - first.size(); ++i) {
        bool match = true;
        // Check if all elements of the first vector match the current segment of the second vector
        for (int j = 0; j < first.size(); j++) {
            if (second[i + j] != first[j]) {
                match = false;  // If there is a mismatch, exit the inner loop
                break;
            }
        }
        if (match) {
            return true;  // Return true if a subpath is found
        }
    }

    return false;  // Return false if no subpath is found
}