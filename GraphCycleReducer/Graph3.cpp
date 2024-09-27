#include "Graph3.h"

Graph3::Graph3(const std::vector<std::vector<int>>& g2_adj, const std::vector<char>& g2_types) : Graph(g2_adj.size()) {
    adjacency_list = g2_adj;
    types = g2_types;
    cycle_components.resize(vertices);
}

std::vector<std::vector<int>> Graph3::GetCycleComponents() const {
    return cycle_components;
}

std::vector<int> Graph3::FindCycle() {
    std::vector<int> cycle;
    std::vector<bool> visited(vertices, false);
    std::vector<int> stack;

    for (int v = 0; v < vertices; v++) {
        if (!visited[v]) {
            if (DFSCycle(v, visited, stack, cycle)) {
                return cycle;
            }
        }
    }
    return {};
}

bool Graph3::DFSCycle(int v, std::vector<bool>& visited, std::vector<int>& stack, std::vector<int>& cycle) {
    visited[v] = true;
    stack.push_back(v);

    for (int u : adjacency_list[v]) {
        if (find(stack.begin(), stack.end(), u) != stack.end()) {
            cycle.insert(cycle.end(), stack.begin() + (find(stack.begin(), stack.end(), u) - stack.begin()), stack.end());
            return true;
        }
        if (!visited[u] && DFSCycle(u, visited, stack, cycle)) {
            return true;
        }
    }
    stack.pop_back();
    return false;
}

void Graph3::RemoveCycles() {
    while (true) {
        std::vector<int> cycle = FindCycle();
        if (cycle.empty()) break; // Exit if no cycles are left

        int v_new = vertices++; // Create a new vertex for replacing the cycle
        adjacency_list.resize(vertices); // Resize the adjacency list for the new vertex
        types.push_back('v');  // Mark the new vertex type as v_new

        // Store the original vertices forming the cycle in cycle_components
        cycle_components.push_back(cycle);

        // Transfer edges from the cycle to the new vertex
        for (int vi : cycle) {
            // Remove incoming edges to cycle vertices and replace them with edges to v_new
            for (int u = 0; u < vertices - 1; ++u) {
                if (u != v_new) {  // Ensure we don't add a self-loop to v_new
                    auto it = find(adjacency_list[u].begin(), adjacency_list[u].end(), vi);
                    if (it != adjacency_list[u].end()) {
                        adjacency_list[u].erase(it); // Remove edge (u, vi)
                        adjacency_list[u].push_back(v_new); // Add edge (u, v_new)
                    }
                }
            }

            // Transfer outgoing edges from cycle vertices to v_new
            for (int u : adjacency_list[vi]) {
                if (u != v_new && find(cycle.begin(), cycle.end(), u) == cycle.end()) {
                    adjacency_list[v_new].push_back(u); // Transfer edges from vi to v_new
                }
            }
        }

        // Clear edges leading to vertices inside the cycle from all other vertices
        for (int u = 0; u < vertices - 1; ++u) {
            adjacency_list[u].erase(remove_if(adjacency_list[u].begin(), adjacency_list[u].end(), [&](int v) {
                return find(cycle.begin(), cycle.end(), v) != cycle.end();
                }), adjacency_list[u].end());
        }

        // Remove edges inside the cycle
        for (int vi : cycle) {
            adjacency_list[vi].clear(); // Clear adjacent vertices for all vertices in the cycle
        }
    }
}
