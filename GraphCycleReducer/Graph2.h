#pragma once
#include "Graph.h"

class Graph2 : public Graph {
public:
    // Constructor for Graph2, initializing types for each vertex
    Graph2(int P);

    // Breadth-First Search (BFS) to find the shortest path between two vertice
    std::vector<int> BFSShortestPath(int start, int end, const std::vector<std::vector<int>>& graph);

    // Unite two paths, finding the overlap or shortest path between them
    std::vector<int> UnitePaths(const std::vector<int>& pathI, const std::vector<int>& pathJ, const std::vector<std::vector<int>>& graph);

    // Create edges for the graph based on the paths and adjacency list from G1
    void CreateEdges(const std::vector<std::vector<int>>& paths, const std::vector<std::vector<int>>& g1_adj);
};