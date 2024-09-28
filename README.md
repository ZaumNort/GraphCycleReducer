# Overview

This project provides a tool for transforming directed cyclic graphs (DCGs) into directed acyclic graphs (DAGs). It consists of two main components: a C++ component for core graph processing and a Python component that provides a graphical user interface (GUI) for visualization and user interaction.

The C++ component handles the transformation of a DCG into a DAG by analyzing and removing cycles, identifying prime paths, and generating intermediate graph structures. The Python component communicates with the C++ program, visualizes the original and transformed graphs, and offers an interactive way to create, load, and analyze graphs.

# Project Structure

## C++ Component

The C++ component is the core of the graph transformation process. The graph processing logic is implemented across a series of classes:

- **`Graph`**: Base class to store vertices and edges.
- **`Graph1`**: Extends `Graph` to find prime paths within the DCG.
- **`Graph2`**: Constructs an intermediate graph, G2, by creating edges between identified paths.
- **`Graph3`**: Analyzes G2 and removes cycles to produce the final DAG (G3).

The process flow within the C++ code follows these main steps:

1. **Finding Prime Paths**: `Graph1` identifies unique, non-overlapping prime paths in the DCG.
2. **Creating Path Connections**: `Graph2` builds G2 by linking prime paths, setting up the structure for transformation.
3. **Cycle Removal and DAG Transformation**: `Graph3` analyzes G2, detects cycles, and removes them to generate the acyclic graph G3.

**`main.cpp`** orchestrates this workflow, reading the initial graph, processing it sequentially through `Graph1`, `Graph2`, and `Graph3`, and outputting the results.

## Python Component

The Python component manages user interaction, graph visualization, and the connection to the C++ transformation logic. It is implemented as a single script, **`GraphCycleReducerPython.py`**, which integrates all the following functionalities:

1. **Graphical User Interface (GUI)**: Built using `tkinter`, the GUI allows users to generate random graphs or load graphs from files. It also provides controls for setting parameters like the number of vertices and edge density.
2. **Graph Creation and Loading**:
   - **Random Graph Generation**: The user can generate a random DCG based on the number of vertices and edge density range. The random graph adheres to the required structure, ensuring a valid path from source (`s`) to target (`t`).
   - **Loading from a File**: Alternatively, users can load a graph from a text file with a specific format. The file contains the number of vertices and edges, followed by edge definitions.
3. **C++ Communication**: Once a graph is generated or loaded, the Python script sends this data to the C++ program for processing. The `send_data_to_cpp()` function handles data transfer, and `parse_cpp_output()` processes the C++ output to extract the results.
4. **Graph Visualization**: The `networkx` library, along with `matplotlib`, is used to visualize the graphs (G1, G2, G3). Users can view the original DCG, the intermediate path graph, and the final DAG.
5. **Output Display**: The results, including paths, edges, and components, are displayed in the GUI and saved to a text file for further analysis.

# How It Works

The process of transforming a DCG into a DAG can be broken down into the following steps:

1. **Graph Creation**: Users can create a DCG via the GUI or by loading a graph from a text file. The Python program validates the graph to ensure it meets the required structure (e.g., paths from `s` to `t`, no self-loops).
2. **Graph Transformation in C++**: The graph data is sent to the C++ component, which processes the graph in the following stages:
   - **G1 Construction**: Identifies and generates prime paths within the DCG.
   - **G2 Path Connections**: Links the prime paths and builds the intermediate path graph G2.
   - **G3 Cycle Removal**: Detects and removes cycles from G2 to create a DAG.
3. **Visualization and Analysis**: The transformed data is returned to the Python script. The GUI displays the original DCG (G1), the intermediate path graph (G2), and the final DAG (G3), along with paths and combined components.

# Code Structure

## Python

### `GraphCycleReducerPython.py`

The main Python script handles all aspects of the user interface, graph generation, communication with the C++ backend, and visualization. It includes:

- **GUI Initialization**: Uses `tkinter` to set up a graphical user interface for interacting with the graph, setting parameters, and visualizing results.
- **Graph Generation and Management**:
  - **Random Graph Generation**: Allows users to create a directed cyclic graph (DCG) based on input parameters (e.g., number of vertices and edge density).
  - **File Loading**: Users can load an existing graph from a formatted text file.
- **Communication with C++**:
  - **`send_data_to_cpp(v, edges)`**: Sends the graph data to the C++ backend for processing and receives the results for visualization.
  - **`parse_cpp_output(output)`**: Parses the output from the C++ transformation, extracting graph information, paths, and vertex components.
- **Graph Visualization**: Uses `networkx` and `matplotlib` to display G1 (initial DCG), G2 (intermediate graph), and G3 (final DAG). Graphs can be interactively manipulated to understand their structure.
  - **`draw_graph(edges, title, v)`**: Draws the current state of the graph.
  - **`is_valid_graph(G, start, target)`**: Checks the validity of the graph structure to ensure a complete path exists from `s` (source) to `t` (target).

### GUI Features

- **Graph Creation**: Generate random graphs or load from a file.
- **Parameter Configuration**: Set the number of vertices and edge density.
- **Run Transformations**: Send graph data to the C++ backend for processing.
- **Result Visualization**: Display the transformed graphs (G1, G2, G3) and their properties, such as edges, paths, and components.

## C++

### Graph Processing Classes

The C++ code is responsible for transforming the DCG into a DAG through three stages:

- **`Graph` Class (Base Class)**:
  - **Files**: `Graph.h`, `Graph.cpp`
  - **Purpose**: Defines the basic structure for storing vertices and edges, providing functionalities for graph traversal, edge addition, and adjacency list management.

- **`Graph1` (Prime Path Identification)**:
  - **Files**: `Graph1.h`, `Graph1.cpp`
  - **Purpose**: Extends the base `Graph` to find and store prime paths within the DCG. These paths are fundamental to the next stage of graph processing.
  - **Key Method**: `GeneratePaths()`: Finds unique paths that cannot be decomposed into subpaths.

- **`Graph2` (Path Graph Construction)**:
  - **Files**: `Graph2.h`, `Graph2.cpp`
  - **Purpose**: Connects the prime paths to build an intermediate graph, G2. Uses breadth-first search (BFS) and path unification methods to generate the correct connections.
  - **Key Methods**:
    - **`CreateEdges()`**: Builds connections between prime paths in G2.
    - **`BFSShortestPath()`**: Utilizes BFS to find paths and ensure proper linking of nodes.

- **`Graph3` (Cycle Removal & DAG Transformation)**:
  - **Files**: `Graph3.h`, `Graph3.cpp`
  - **Purpose**: Detects and removes cycles in G2 to produce the final DAG (G3). Uses depth-first search (DFS) to identify cycles and merges nodes to eliminate cyclic structures.
  - **Key Method**: `RemoveCycles()`: Analyzes G2 for cycles and modifies the graph structure to create a DAG.

### `main.cpp` Workflow

1. **Graph Input**: Reads the graph from input data provided by the Python script.
2. **Prime Path Generation**: Uses `Graph1` to identify and generate prime paths.
3. **Construct Intermediate Graph**: `Graph2` builds an intermediate graph (G2) by creating edges between paths.
4. **Cycle Removal**: `Graph3` transforms G2 into G3 by removing cycles, resulting in a DAG.
5. **Output**: The results are sent back to the Python script for visualization and analysis.

# Usage and Setup

## Requirements

- **Python Libraries**: `tkinter`, `networkx`, `matplotlib`.
- **C++ Compiler**: Needed to compile and run the C++ code.

## Installing Dependencies

To install all necessary Python dependencies, use:
```bash
pip install -r requirements.txt
```
Then, compile the C++ code with your preferred compiler.

## Running the Program
1. Run the Python script GraphCycleReducerPython.py.
2. Use the GUI to create or load a graph.
3. Send the graph for processing to transform the DCG into a DAG.
4. Visualize the results in the GUI.
# Graph Data Format for Text File
## Format
1. First Line: Specifies the number of vertices v.
2. Subsequent Lines: Each line represents a directed edge in the format from_node to_node.
### Example
```bash
7
0 1
1 2
1 3
3 4
4 5
4 1
5 4
2 6
```
### Explanation

- **First Line**: `7` — Specifies that the graph contains 7 vertices (labeled from `0` to `6`).
- **Edge Lines**: Each subsequent line defines a directed edge between two vertices:
  - `0 1` — An edge from vertex `0` to vertex `1`.
  - `1 2` — An edge from vertex `1` to vertex `2`.
  - `2 3` — An edge from vertex `2` to vertex `3`.
  - `3 4` — An edge from vertex `3` to vertex `4`.
  - `4 5` — An edge from vertex `4` to vertex `5`.
  - `5 6` — An edge from vertex `5` to vertex `6`.

The file format allows the program to correctly parse the vertices and edges to construct the initial DCG.

### Graph Validity Rules

For the algorithms to work correctly and produce a valid transformation of a DCG to a DAG, the input graph must meet the following conditions:

- **Source (s) and Target (t) Vertex Identification**: The graph must have a source vertex s with a fixed index 0, and a target vertex t with index n-1 (where n is the total number of vertices).
- **No Self-loops**: The graph should not contain cycles where a vertex has an edge to itself (e.g., 1 1).
- **Valid Path from s to t**: There must be a clear path from the start vertex (s) to the end vertex (t) that passes through all vertices in the graph.
- **No Disconnected Components**: All edges must participate in a path that connects s to t. There should be no isolated vertices or disconnected subgraphs.

If the input graph does not meet these conditions, the algorithm may not produce the correct result, and the transformation from DCG to DAG may fail.

## Performance Considerations

The transformation algorithm's performance is highly dependent on the input graph's size and complexity. 
For large graphs or high edge densities, the computation may be slow or resource-intensive, potentially leading to long processing times or memory issues.

# Results and Output
After C++ processing, the Python script allows visualization of:

- G1 (Initial DCG): Displays all edges and vertices of the initial graph.
- G2 (Path Graph): Graph constructed based on the prime paths.
- G3 (Final DAG): Transformed graph without cycles.
The results are also outputted in textual form, including information about edges, components, and identified paths.

## Transformation Algorithms

The transformation algorithms implemented in this project are based on the work of A. Dwarakanath, A. Jankiti titled *"Minimum Number of Test Paths for Prime Path and
Other Structural Coverage Criteria"*, 26th IFIP International Conference on Testing Software and Systems
(ICTSS), Sep 2014, Madrid, Spain. pp.63-79, https://doi.org/10.1007/978-3-662-44857-1_5 hal-01405275

