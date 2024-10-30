import tkinter as tk
from tkinter import filedialog
import subprocess
import networkx as nx
import matplotlib.pyplot as plt
import random
import time

# Function to send graph data to the C++ program
def send_data_to_cpp(v, edges):
    # Prepare input data for the C++ program
    input_data = f"{v} {len(edges)}\n"
    input_data += "\n".join(f"{from_node} {to_node}" for from_node, to_node in edges)
    
    # Run the C++ program via subprocess and send the data to its input
    result = subprocess.run(
        ["./GraphCycleReducer"],
        input=input_data,
        text=True,
        capture_output=True
    )
    
    # Return stdout for further processing
    return result.stdout

# Function to parse the C++ output into G2, G3, paths, and v-components
def parse_cpp_output(output):
    # Split the output into lines
    lines = output.strip().split('\n')
    graphs_data = {}
    
    section_map = {
        "G2:": "G2",
        "G3:": "G3",
        "Paths:": "Paths",
        "v components:": "v_components"
    }
    
    current_section = None
    current_data = []
    for line in lines:
        # Check for new sections
        if line.strip() in section_map:
            # Save the data from the previous section if it exists
            if current_section:
                graphs_data[current_section] = current_data
            # Update to the new section
            current_section = section_map[line.strip()]
            current_data = []
        else:
            # Parse content based on section
            if current_section in ["G2", "G3"]:
                if ':' in line:
                    node_info, neighbors = line.split(':', 1)
                    node = node_info.strip()
                    for neighbor in neighbors.strip().split():
                        current_data.append((node, neighbor))
            elif current_section == "Paths":
                # Parsing paths
                current_data.append(line.split())
            elif current_section == "v_components":
                if ':' in line:
                    v_name, components = line.split(':', 1)
                    components_list = components.strip().split()
                    if components_list:
                        current_data.append((v_name.strip(), components_list))
    # Store the last section parsed
    if current_section:
        graphs_data[current_section] = current_data
    return graphs_data

# Function to draw a graph
def draw_graph(edges, title, v=None):
    G = nx.DiGraph()
    if v is not None:
        G.add_nodes_from(range(v))
    G.add_edges_from(edges)

    # Assign colors based on node type (s, t, p, v)
    node_colors = {
        's': 'red',
        't': 'blue',
        'p': 'green',
        'v': 'orange'
    }

    # Determine node types
    nodes_order = list(G.nodes)
    node_types = {}
    for node in nodes_order:
        if isinstance(node, int) and v is not None:
            # Determine node type based on position
            if node == 0:
                node_type = 's'
            elif node == v - 1:
                node_type = 't'
            else:
                node_type = 'p'
        else:
            node_type = node[0] if isinstance(node, str) else 'p'
        node_types[node] = node_type

    # Create color map
    node_color_map = [node_colors.get(node_types[node], 'gray') for node in nodes_order]

    # Initial positions for nodes
    pos = nx.spring_layout(G)  # Generate positions for the nodes
    node_positions = pos.copy()

    # Create figure and axis
    fig, ax = plt.subplots(figsize=(8, 6))
    plt.title(title)

    # Draw the graph with specific colors for each type
    nx.draw_networkx_nodes(G, node_positions, ax=ax, node_color=node_color_map, node_size=500)
    nx.draw_networkx_edges(G, node_positions, ax=ax, arrowstyle='->', arrows=True)
    nx.draw_networkx_labels(G, node_positions, ax=ax, font_weight='bold')

    # Enable interactive mode
    plt.ion()

    # Function to handle mouse click events
    def on_click(event):
        if event.inaxes is None:
            return
        # Get mouse position
        x, y = event.xdata, event.ydata
        # Find if a node is clicked
        for node, (xn, yn) in node_positions.items():
            dx = xn - x
            dy = yn - y
            distance = (dx*dx + dy*dy)**0.5
            if distance < 0.1:
                def on_motion(event):
                    if event.inaxes is None:
                        return
                    # Update node position
                    node_positions[node] = (event.xdata, event.ydata)
                    ax.clear()
                    # Draw the graph with updated positions
                    nx.draw_networkx_nodes(G, node_positions, ax=ax, node_color=node_color_map, node_size=500)
                    nx.draw_networkx_edges(G, node_positions, ax=ax, arrowstyle='->', arrows=True)
                    nx.draw_networkx_labels(G, node_positions, ax=ax, font_weight='bold')
                    plt.title(title)
                    fig.canvas.draw()

                def on_release(event):
                    fig.canvas.mpl_disconnect(motion_cid)
                    fig.canvas.mpl_disconnect(release_cid)

                motion_cid = fig.canvas.mpl_connect('motion_notify_event', on_motion)
                release_cid = fig.canvas.mpl_connect('button_release_event', on_release)
                break

    cid = fig.canvas.mpl_connect('button_press_event', on_click)

    plt.show()
    plt.ioff()

# Function to determine node types based on number of vertices
def determine_node_types(v):
    return ['s'] + ['p'] * (v - 2) + ['t']


# Function to visualize paths
def print_paths(paths, text_widget):
    text_widget.insert(tk.END, "Paths:\n")
    for index, path in enumerate(paths):
        # Prefix the path with "p<index>:"
        path_str = f"p{index}: " + " -> ".join(path) + "\n"
        text_widget.insert(tk.END, path_str)

# Function to visualize v-components
def print_v_components(v_components, text_widget):
    text_widget.insert(tk.END, "v components:\n")
    for v_name, components in v_components:
        if components:
            comp_str = f"{v_name}: " + " ".join(components) + "\n"
            text_widget.insert(tk.END, comp_str)

# Function to check if the graph is valid
def is_valid_graph(G, start, target):
    # Check if all nodes are reachable from start
    visited_from_start = set(nx.dfs_preorder_nodes(G, start))
    if len(visited_from_start) != G.number_of_nodes():
        return False

    # Check if target is reachable from all nodes (reverse graph)
    G_rev = G.reverse()
    visited_to_target = set(nx.dfs_preorder_nodes(G_rev, target))
    if len(visited_to_target) != G.number_of_nodes():
        return False

    return True

# Function to generate a random graph
def random_graph(num_vertices, min_edges, max_edges):
    while True:
        edges = []

        # Generate all possible edges between all vertices except entering 's' and exiting 't'
        intermediate_vertices = list(range(1, num_vertices - 1))
        all_vertices = [0] + intermediate_vertices + [num_vertices - 1]

        possible_edges = []
        for i in all_vertices:
            for j in all_vertices:
                if i != j and j != 0 and i != num_vertices - 1:
                    possible_edges.append((i, j))

        num_edges_to_keep = random.randint(min_edges, max_edges)
        edges = random.sample(possible_edges, num_edges_to_keep)

        # Create a directed graph
        G = nx.DiGraph()
        G.add_nodes_from(all_vertices)
        G.add_edges_from(edges)

        # Check if the generated graph meets the criteria
        if is_valid_graph(G, start=0, target=num_vertices - 1):
            break

    return edges

def dfs_cycle(adj,u,visited, stack):
    visited[u] = True
    stack[u] = True
    
    for x in adj[u]:
        if not visited[x]:
            if dfs_cycle(adj, x, visited, stack):
                return True
        elif stack[x]:
            return True

    stack[u] = False
    return False


def directed_cycle(edges):
    adj_list = {}
    
    for from_node, to_node in edges:
        if from_node not in adj_list:
            adj_list[from_node] = []
        adj_list[from_node].append(to_node)

        if to_node not in adj_list:
            adj_list[to_node] = []

    
    visited = {key: False for key in adj_list}  
    stack = {key: False for key in adj_list}

    for node in adj_list:
        if not visited[node]:
           if dfs_cycle(adj_list, node, visited, stack):
            return True

    return False


# Main application class
class GraphApp:
    def __init__(self, master):
        self.master = master
        master.title("GraphCycleReducer")
        master.geometry("800x600")

        # Number of vertices input
        self.vertices_label = tk.Label(master, text="Number of vertices:")
        self.vertices_label.pack()
        self.vertices_entry = tk.Entry(master)
        self.vertices_entry.insert(0, "7")
        self.vertices_entry.pack()
        self.vertices_entry.bind("<KeyRelease>", self.update_edge_sliders)

        # Min Edges label
        self.min_edges_label = tk.Label(master, text="Min Edges:")
        self.min_edges_label.pack()

        # Min Edges slider
        self.min_edges_slider = tk.Scale(master, from_=0, to=1, orient=tk.HORIZONTAL, length=400)
        self.min_edges_slider.pack()
        self.min_edges_slider.bind("<ButtonRelease-1>", self.sync_max_edges)

        # Max Edges label
        self.max_edges_label = tk.Label(master, text="Max Edges:")
        self.max_edges_label.pack()

        # Max Edges slider
        self.max_edges_slider = tk.Scale(master, from_=0, to=1, orient=tk.HORIZONTAL, length=400)
        self.max_edges_slider.pack()
        self.max_edges_slider.bind("<ButtonRelease-1>", self.sync_min_edges)

        # Generate graph button
        self.generate_button = tk.Button(master, text="Generate Graph", command=self.generate_graph)
        self.generate_button.pack()

        # Load graph button
        self.load_button = tk.Button(master, text="Load Graph from File", command=self.load_graph)
        self.load_button.pack()

        # Text widget to display paths and v-components
        self.text_output = tk.Text(master, height=10)
        self.text_output.pack()

        # Initialize sliders
        self.update_edge_sliders()

    def update_edge_sliders(self, event=None):
        try:
            v = int(self.vertices_entry.get())
            min_possible_edges = v - 1  # Minimum edges to connect all nodes in a path
            total_possible_edges =(v - 1)+((v - 2) * (v - 2))  # Total possible edges excluding entering 's' and exiting 't'

            # Update sliders
            self.min_edges_slider.config(from_=min_possible_edges, to=total_possible_edges, resolution=1)
            self.max_edges_slider.config(from_=min_possible_edges, to=total_possible_edges, resolution=1)

            if self.min_edges_slider.get() < min_possible_edges or self.min_edges_slider.get() > total_possible_edges:
                self.min_edges_slider.set(min_possible_edges)
            if self.max_edges_slider.get() < self.min_edges_slider.get():
                self.max_edges_slider.set(self.min_edges_slider.get())
            if self.max_edges_slider.get() > total_possible_edges:
                self.max_edges_slider.set(total_possible_edges)

        except ValueError:
            pass

    def sync_max_edges(self, event=None):
        # Ensure max_edges is not less than min_edges
        if self.max_edges_slider.get() < self.min_edges_slider.get():
            self.max_edges_slider.set(self.min_edges_slider.get())

    def sync_min_edges(self, event=None):
        # Ensure min_edges is not greater than max_edges
        if self.min_edges_slider.get() > self.max_edges_slider.get():
            self.min_edges_slider.set(self.max_edges_slider.get())

    def generate_graph(self):
        v = int(self.vertices_entry.get())
        min_edges = self.min_edges_slider.get()
        max_edges = self.max_edges_slider.get()

        edges = random_graph(v, min_edges, max_edges)
        self.edges = edges
        self.v = v

        # Draw G1 using draw_graph
        draw_graph(edges, "Graph G1", v=self.v)

        # Process graph through C++ program
        self.process_graph()

    def load_graph(self):
        file_path = filedialog.askopenfilename()
        if file_path:
            with open(file_path, 'r') as f:
                lines = f.readlines()
                v = int(lines[0].strip())
                edges = []
                for line in lines[1:]:
                    from_node, to_node = map(int, line.strip().split())
                    edges.append((from_node, to_node))
            self.edges = edges
            self.v = v

            # Draw G1
            draw_graph(edges, "Graph G1", v=self.v)

            # Process graph through C++ program
            self.process_graph()

    def process_graph(self):
        start_time = time.time()
        # Send data to C++ program and capture its output
        cpp_output = send_data_to_cpp(self.v, self.edges)
        # Parse the output to extract G2, G3, and paths
        graphs_data = parse_cpp_output(cpp_output)
        end_time = time.time()
        elapsed_time = end_time - start_time
        print(f"Elapsed time: {elapsed_time:.5f} seconds")
        # Clear previous text
        self.text_output.delete(1.0, tk.END)
        # Print paths
        if "Primal Paths" in graphs_data:
            print_paths(graphs_data["Paths"], self.text_output)

        # Print v-components
        if "v_components" in graphs_data:
            print_v_components(graphs_data["v_components"], self.text_output)

        # Draw G2 and G3
        if "G2" in graphs_data:
            draw_graph(graphs_data["G2"], "Graph G2")
            if directed_cycle(graphs_data["G2"]):
                print("G2 Contains Cycle")
            else:
                print("No Cycle in G2")
        if "G3" in graphs_data:
            draw_graph(graphs_data["G3"], "Graph G3")
            if directed_cycle(graphs_data["G3"]):
                print("G3 Contains Cycle")
            else:
                print("No Cycle in G3")
        
        # Save data to a text file
        with open('graph_data.txt', 'w') as f:
            if "G2" in graphs_data:
                f.write("G2 adjacency list:\n")
                G2_adj = {}
                for edge in graphs_data["G2"]:
                    from_node, to_node = edge
                    G2_adj.setdefault(from_node, []).append(to_node)
                for node in G2_adj:
                    f.write(f"{node}: {' '.join(G2_adj[node])}\n")
                f.write("\n")
            if "G3" in graphs_data:
                f.write("G3 adjacency list:\n")
                G3_adj = {}
                for edge in graphs_data["G3"]:
                    from_node, to_node = edge
                    G3_adj.setdefault(from_node, []).append(to_node)
                for node in G3_adj:
                    f.write(f"{node}: {' '.join(G3_adj[node])}\n")
                f.write("\n")
            if "Paths" in graphs_data:
                f.write("Primal Paths:\n")
                for index, path in enumerate(graphs_data["Paths"]):
                    path_str = f"p{index}: " + " -> ".join(path) + "\n"
                    f.write(path_str)
                f.write("\n")
            if "v_components" in graphs_data:
                f.write("v components:\n")
                for v_name, components in graphs_data["v_components"]:
                    if components:
                        comp_str = f"{v_name}: " + " ".join(components) + "\n"
                        f.write(comp_str)
                f.write("\n")

def main():
    root = tk.Tk()
    app = GraphApp(root)
    root.mainloop()

if __name__ == "__main__":
    main()
