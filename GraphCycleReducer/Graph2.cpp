#include "Graph2.h"
#include <queue>

Graph2::Graph2(int P) : Graph(P) {
    types.resize(P);
    for (int i = 0; i < P; i++) {
        types[i] = 'p';
    }
    types[P - 2] = 's';
    types[P - 1] = 't';
}

std::vector<int> Graph2::BFSShortestPath(int start, int end, const std::vector<std::vector<int>>& graph) {
    std::vector<int> visited(graph.size(), 0);
    std::vector<int> from(graph.size(), -1);
    std::queue<int> q;

    visited[start] = 1;
    q.push(start);

    while (!q.empty()) {
        int v = q.front();
        q.pop();

        for (int to : graph[v]) {
            if (!visited[to]) {
                visited[to] = 1;
                from[to] = v;
                q.push(to);

                if (to == end) {
                    std::vector<int> path;
                    for (int v = end; v != -1; v = from[v]) {
                        path.push_back(v);
                    }
                    reverse(path.begin(), path.end());
                    return path;
                }
            }
        }
    }
}

std::vector<int> Graph2::UnitePaths(const std::vector<int>& pathI, const std::vector<int>& pathJ, const std::vector<std::vector<int>>& graph) {
    std::vector<int> unitedPaths;
    int overlapLength = 0;

    // Find the largest overlap
    for (int i = 0; i < pathI.size(); ++i) {
        bool match = true;
        for (int j = 0; j + i < pathI.size() && j < pathJ.size(); ++j) {
            if (pathI[i + j] != pathJ[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            overlapLength = pathI.size() - i;  // Find the length of the overlap
            break;
        }
    }

    // If an overlap is found, merge with the overlap
    if (overlapLength > 0) {
        unitedPaths.insert(unitedPaths.end(), pathI.begin(), pathI.end() - overlapLength);  // Part of pathI without the overlap
        unitedPaths.insert(unitedPaths.end(), pathJ.begin(), pathJ.end());  // Entire pathJ
    }
    // If no overlap, find the shortest path between the end of pathI and the start of pathJ
    else {
        std::vector<int> shortestPath = BFSShortestPath(pathI.back(), pathJ.front(), graph);
        if (!shortestPath.empty()) {
            unitedPaths.insert(unitedPaths.end(), pathI.begin(), pathI.end());  // Entire pathI
            unitedPaths.insert(unitedPaths.end(), shortestPath.begin() + 1, shortestPath.end() - 1);  // Shortest path between pathI and pathJ (without duplicate vertices)
            unitedPaths.insert(unitedPaths.end(), pathJ.begin(), pathJ.end());  // Entire pathJ
        }
    }

    return unitedPaths;
}

void Graph2::CreateEdges(const std::vector<std::vector<int>>& paths, const std::vector<std::vector<int>>& g1_adj) {
    for (int i = 0; i < vertices - 1; i++) {
        for (int j = 0; j < vertices; j++) {
            if (i != j && j != vertices - 2) {
                std::vector<int> PathIJ = UnitePaths(paths[i], paths[j], g1_adj);
                if (!PathIJ.empty()) {
                    // Check that only pi and pj are subpaths in PathIJ
                    bool validEdge = true;
                    for (int k = 0; k < vertices - 2; k++) {
                        if (k != i && k != j && IsSubPath(paths[k], PathIJ)) {
                            validEdge = false;
                            break;  // If another path is a subpath, stop checking
                        }
                    }

                    // Add the edge only if only pi and pj are subpaths
                    if (validEdge) {
                        AddEdge(i, j);
                    }
                }
            }
        }
    }
}