#pragma once
#include "Graph.h"

class Graph3 : public Graph {
private:
    std::vector<std::vector<int>> cycle_components; // Store which vertices form each 'v' vertex
public:
    Graph3(const std::vector<std::vector<int>>& g2_adj, const std::vector<char>& g2_types);

    std::vector<std::vector<int>> GetCycleComponents() const;

    // Function to find a cycle in G3
    std::vector<int> FindCycle();

    // DFS for cycle detection
    bool DFSCycle(int v, std::vector<bool>& visited, std::vector<int>& stack, std::vector<int>& cycle);

    // Transform G3: remove cycles
    void RemoveCycles();

};
