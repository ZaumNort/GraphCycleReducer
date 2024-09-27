#pragma once
#include "Graph.h"

class Graph1 : public Graph {
public:
    Graph1(int V);

    // Recursive function to find paths
    void FindPaths(int start, int current, std::vector<int>& path, std::vector<std::vector<int>>& paths, std::vector<bool>& visited);

    // Function to generate all paths
    std::vector<std::vector<int>> GeneratePaths();

};