#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

// Edge structure for adjacency list
typedef struct Edge {
    int dest;              // Destination vertex
    double weight;         // Edge weight (1.0 for unweighted)
    struct Edge* next;     // Next edge in list
} Edge;

// Vertex coordinates for heuristic calculations
typedef struct {
    double x;
    double y;
} Coordinates;

// Graph structure
typedef struct {
    int num_vertices;      // Number of vertices
    int num_edges;         // Number of edges
    bool is_weighted;      // Is graph weighted?
    bool is_directed;      // Is graph directed?
    Edge** adj_list;       // Array of adjacency lists
    Coordinates* coords;   // Coordinates for each vertex (optional, for A*)
} Graph;

// Path result structure
typedef struct {
    int* path;             // Array of vertices in path
    int path_length;       // Number of vertices in path
    double total_weight;   // Total weight of path
    double time_ms;        // Execution time in milliseconds
    const char* algorithm; // Algorithm name
    bool found;            // Was path found?
} PathResult;

// Graph creation and destruction
Graph* graph_create(int num_vertices, bool is_weighted, bool is_directed);
void graph_destroy(Graph* graph);

// Graph operations
bool graph_add_edge(Graph* graph, int src, int dest, double weight);
bool graph_remove_edge(Graph* graph, int src, int dest);
bool graph_add_vertex(Graph* graph);
bool graph_remove_vertex(Graph* graph, int vertex);
void graph_print(const Graph* graph);
bool graph_is_valid_vertex(const Graph* graph, int vertex);

// Coordinate operations
bool graph_set_coordinates(Graph* graph, int vertex, double x, double y);
bool graph_has_coordinates(const Graph* graph);
double graph_euclidean_distance(const Graph* graph, int v1, int v2);
double graph_manhattan_distance(const Graph* graph, int v1, int v2);

// Path result operations
PathResult* path_result_create(void);
void path_result_destroy(PathResult* result);
void path_result_print(const PathResult* result);

#endif // GRAPH_H
