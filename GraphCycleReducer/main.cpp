#include <iostream>
#include <vector>
#include "Graph1.h"
#include "Graph2.h"
#include "Graph3.h"

// Function to read graph data from standard input
void ReadGraph(Graph1& g1) {
    int V, E;
    std::cin >> V >> E;  // Read the number of vertices and edges
    g1 = Graph1(V);
    for (int i = 0; i < E; ++i) {
        int from, to;
        std::cin >> from >> to;  // Read edges of the graph
        g1.AddEdge(from, to);
    }
}

// Function to print the graphs G2 and G3 with their types
void PrintGraph(const std::vector<std::vector<int>>& adj, const std::vector<char>& types) {
    int v_counter = 0;  // Counter for vertices of type 'v'
    std::vector<int> v_mapping(adj.size(), -1);  // Mapping from vertex index to its 'v' numbering

    // Assign numbers to 'v' vertices
    for (int i = 0; i < adj.size(); ++i) {
        if (types[i] == 'v') {
            v_mapping[i] = v_counter++;
        }
    }

    for (int i = 0; i < adj.size(); ++i) {
        // Print the current vertex label
        if (types[i] == 's' || types[i] == 't') {
            std::cout << types[i] << ": ";
        }
        else if (types[i] == 'v') {
            std::cout << "v" << v_mapping[i] << ": ";
        }
        else {
            std::cout << "p" << i << ": ";
        }

        // Print adjacent vertices with their types
        for (int neighbor : adj[i]) {
            // Print label for the adjacent vertex
            if (types[neighbor] == 's' || types[neighbor] == 't') {
                std::cout << types[neighbor] << " ";
            }
            else if (types[neighbor] == 'v') {
                std::cout << "v" << v_mapping[neighbor] << " ";  // Use the v_mapping to ensure correct numbering
            }
            else {
                std::cout << "p" << neighbor << " ";
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    // Initialize an empty Graph1 object with 0 vertices initially
    Graph1 g1(0);
    // Read graph data (number of vertices, edges, and the edges themselves) from standard input
    ReadGraph(g1);

    // Generate the list of primal paths from the graph G1
    std::vector<std::vector<int>> paths = g1.GeneratePaths();

    // Create the transformed graph G2 using the paths from G1
    Graph2 g2(paths.size());
    g2.CreateEdges(paths, g1.get_adjacency_list());

    // Create DAG G3 from G2
    Graph3 g3(g2.get_adjacency_list(), g2.get_types());
    g3.RemoveCycles();

    std::cout << "G2:" << std::endl;
    PrintGraph(g2.get_adjacency_list(), g2.get_types());

    std::cout << "G3:" << std::endl;
    PrintGraph(g3.get_adjacency_list(), g3.get_types());

    std::cout << "v components:" << std::endl;
    PrintGraph(g3.GetCycleComponents(), g3.get_types());

    // Print the generated paths, only those with more than one node (excluding s and t individually)
    std::cout << "Paths:" << std::endl;
    for (const auto& path : paths) {
        if (path.size() > 1) {
            for (int node : path) {
                std::cout << node << " ";
            }
            std::cout << std::endl;
        }
    }

    return 0;
}
