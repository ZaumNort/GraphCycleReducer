#pragma once
#include <vector>

// Base class for a graph
class Graph {
protected:
    int vertices; // Number of vertices
    std::vector<std::vector<int>> adjacency_list; // Adjacency list
    std::vector<char> types;

public:
    Graph(int V);

    // Get the types of vertices (e.g., prime, source, sink)
    const std::vector<char>& get_types() const;

    // Function to add edges
    void AddEdge(int v, int w);

    // Getter for the adjacency list
    const std::vector<std::vector<int>>& get_adjacency_list() const;

    // Getter for the number of vertices
    int get_vertices() const;

    bool IsSubPath(std::vector<int> first, std::vector<int> second);
};