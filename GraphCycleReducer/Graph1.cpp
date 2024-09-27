#include "Graph1.h"
#include <algorithm>
#include <iostream>

Graph1::Graph1(int V) : Graph(V) {}

void Graph1::FindPaths(int start, int current, std::vector<int>& path, std::vector<std::vector<int>>& paths, std::vector<bool>& visited) {
    path.push_back(current); // Add the current vertex to the path

    // If we hit a end (no more adjacent vertices to continue)
    if (adjacency_list[current].empty() ||
        all_of(adjacency_list[current].begin(), adjacency_list[current].end(), [&](int next) { return visited[next]; }) ||
        current == path.front()) {
        paths.push_back(path);  // Add the path as a candidate
    }
    else {
        // Continue extending the path
        for (int next : adjacency_list[current]) {
            if (!visited[next]) {
                visited[next] = true;  // Mark the vertex as visited
                FindPaths(start, next, path, paths, visited);  // Recursively extend the path
                visited[next] = false;  // Backtrack to the previous split
            }
        }
    }

    path.pop_back();  // Backtrack
}

std::vector<std::vector<int>> Graph1::GeneratePaths() {
    std::vector<std::vector<int>> allPaths;

    // For each vertex, start searching for paths from each edge
    for (int v = 0; v < vertices; ++v) {
        for (int neighbor : adjacency_list[v]) {
            std::vector<bool> visited(vertices, false);
            std::vector<int> path;
            path.push_back(v);       // Start vertex in the path, but not marked as visited
            visited[neighbor] = true;  // Mark the second vertex as visited
            FindPaths(v, neighbor, path, allPaths, visited);
        }
    }

    // Sort paths by size
    sort(allPaths.begin(), allPaths.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        return a.size() > b.size();
        });

    // Remove subpaths
    std::vector<std::vector<int>> filteredPaths;
    for (int i = 0; i < allPaths.size(); ++i) {
        bool isSubpath = false;
        for (int j = 0; j < filteredPaths.size(); ++j) {
            if (IsSubPath(allPaths[i], filteredPaths[j])) {
                isSubpath = true;
                break;  // Stop if a subpath is found
            }
        }
        if (!isSubpath) {
            filteredPaths.push_back(allPaths[i]);  // Add the path if it's not a subpath
        }
    }

    //Add s and t at the end for the correct functioning of the algorithms
    std::vector<int> spath = { 0 };
    std::vector<int> tpath = { vertices - 1 };
    filteredPaths.push_back(spath);
    filteredPaths.push_back(tpath);

    return filteredPaths;
}