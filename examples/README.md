# GraphPath Examples

This directory contains example graph files for testing the GraphPath application.

## File Format

Each example file contains input data in the following format:

```
<number_of_vertices>
<is_weighted: y/n>
<is_directed: y/n>
<number_of_edges>
<edge_1: src dest [weight]>
<edge_2: src dest [weight]>
...
<add_coordinates: y/n>
[coordinates if yes: x y for each vertex]
<start_vertex>
<end_vertex>
```

## Examples

### 1. `simple_weighted.txt`
- **Type**: Weighted, Undirected
- **Vertices**: 5
- **Edges**: 7
- **Best for**: Testing Dijkstra, A* (Euclidean), A* (Manhattan), Bellman-Ford
- **Coordinates**: Yes (enables A* with heuristics)
- **Path**: From vertex 0 to vertex 4

### 2. `grid_graph.txt`
- **Type**: Weighted, Undirected
- **Vertices**: 9 (3x3 grid)
- **Edges**: 12
- **Best for**: Testing A* with Manhattan distance (perfect for grid layouts)
- **Coordinates**: Yes (grid layout 0-200 range)
- **Path**: From vertex 0 (top-left) to vertex 8 (bottom-right)

### 3. `negative_weights.txt`
- **Type**: Weighted, Directed
- **Vertices**: 5
- **Edges**: 7
- **Best for**: Testing Bellman-Ford (handles negative weights)
- **Note**: Dijkstra **cannot** handle negative weights
- **Coordinates**: No
- **Path**: From vertex 0 to vertex 4

### 4. `unweighted_simple.txt`
- **Type**: Unweighted, Undirected
- **Vertices**: 6
- **Edges**: 8
- **Best for**: Comparing BFS and DFS algorithms
- **Coordinates**: No (not needed for unweighted graphs)
- **Path**: From vertex 0 to vertex 5

## How to Use

### CLI Application

Run any example with:

```bash
./build/graphpath < examples/simple_weighted.txt
```

Or run manually and follow the prompts.

### GUI Application

1. Run the GUI: `./build/graphpath-gui`
2. Create a graph using the controls
3. The GUI will automatically use vertex positions as coordinates for A*
4. Run pathfinding to compare all algorithms

## Algorithm Selection

The application automatically selects appropriate algorithms based on graph type:

**For Weighted Graphs:**
- Dijkstra
- Bellman-Ford
- A* (Zero heuristic) - always available
- A* (Euclidean) - if coordinates are provided
- A* (Manhattan) - if coordinates are provided

**For Unweighted Graphs:**
- BFS (Breadth-First Search)
- DFS (Depth-First Search)

## Creating Your Own Examples

You can create your own graph files following the format above. Key points:

1. Vertices are numbered from 0 to (n-1)
2. For weighted graphs, include weight after src and dest
3. For unweighted graphs, omit the weight
4. Coordinates are optional but enable A* with Euclidean/Manhattan heuristics
5. Coordinates should be in the range 0-1000 for best results

## Testing Specific Algorithms

- **Test A* Euclidean**: Use graphs with coordinates (e.g., `simple_weighted.txt`)
- **Test A* Manhattan**: Use grid-based graphs (e.g., `grid_graph.txt`)
- **Test Bellman-Ford**: Use graphs with negative weights (e.g., `negative_weights.txt`)
- **Test BFS vs DFS**: Use unweighted graphs (e.g., `unweighted_simple.txt`)
