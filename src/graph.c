#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Create a new graph
Graph* graph_create(int num_vertices, bool is_weighted, bool is_directed) {
    if (num_vertices <= 0) {
        fprintf(stderr, "Error: Number of vertices must be positive\n");
        return NULL;
    }

    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        fprintf(stderr, "Error: Memory allocation failed for graph\n");
        return NULL;
    }

    graph->num_vertices = num_vertices;
    graph->num_edges = 0;
    graph->is_weighted = is_weighted;
    graph->is_directed = is_directed;

    // Allocate adjacency list array
    graph->adj_list = (Edge**)calloc(num_vertices, sizeof(Edge*));
    if (!graph->adj_list) {
        fprintf(stderr, "Error: Memory allocation failed for adjacency list\n");
        free(graph);
        return NULL;
    }

    // Initialize coordinates to NULL (optional feature)
    graph->coords = NULL;

    return graph;
}

// Destroy graph and free memory
void graph_destroy(Graph* graph) {
    if (!graph) return;

    // Free all edges in adjacency lists
    for (int i = 0; i < graph->num_vertices; i++) {
        Edge* current = graph->adj_list[i];
        while (current) {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph->adj_list);
    free(graph->coords);  // Free coordinates if allocated
    free(graph);
}

// Check if vertex is valid
bool graph_is_valid_vertex(const Graph* graph, int vertex) {
    return graph && vertex >= 0 && vertex < graph->num_vertices;
}

// Add a vertex to the graph (dynamic resizing)
bool graph_add_vertex(Graph* graph) {
    if (!graph) {
        fprintf(stderr, "Error: Graph is NULL\n");
        return false;
    }

    int new_count = graph->num_vertices + 1;

    // Reallocate adjacency list array
    Edge** new_adj_list = (Edge**)realloc(graph->adj_list, new_count * sizeof(Edge*));
    if (!new_adj_list) {
        fprintf(stderr, "Error: Memory allocation failed for new vertex\n");
        return false;
    }

    graph->adj_list = new_adj_list;
    graph->adj_list[graph->num_vertices] = NULL;  // Initialize new vertex
    graph->num_vertices = new_count;

    return true;
}

// Remove a vertex from the graph
bool graph_remove_vertex(Graph* graph, int vertex) {
    if (!graph_is_valid_vertex(graph, vertex)) {
        fprintf(stderr, "Error: Invalid vertex %d\n", vertex);
        return false;
    }

    // Free all edges from this vertex
    Edge* current = graph->adj_list[vertex];
    while (current) {
        Edge* temp = current;
        current = current->next;

        // Update edge count
        graph->num_edges--;
        if (!graph->is_directed) {
            // For undirected, each edge was counted once
            // but we need to remove both directions
        }

        free(temp);
    }
    graph->adj_list[vertex] = NULL;

    // Remove all edges TO this vertex from other vertices
    for (int i = 0; i < graph->num_vertices; i++) {
        if (i == vertex) continue;

        Edge** edge_ptr = &graph->adj_list[i];
        while (*edge_ptr) {
            if ((*edge_ptr)->dest == vertex) {
                Edge* to_remove = *edge_ptr;
                *edge_ptr = (*edge_ptr)->next;
                free(to_remove);

                if (graph->is_directed) {
                    graph->num_edges--;
                }
            } else {
                edge_ptr = &(*edge_ptr)->next;
            }
        }
    }

    // Shift all vertices after the removed one
    for (int i = vertex; i < graph->num_vertices - 1; i++) {
        graph->adj_list[i] = graph->adj_list[i + 1];
    }

    // Update all edge destinations that reference vertices after the removed one
    for (int i = 0; i < graph->num_vertices - 1; i++) {
        Edge* edge = graph->adj_list[i];
        while (edge) {
            if (edge->dest > vertex) {
                edge->dest--;
            }
            edge = edge->next;
        }
    }

    graph->num_vertices--;

    // Optionally shrink the adjacency list array
    if (graph->num_vertices > 0) {
        Edge** new_adj_list = (Edge**)realloc(graph->adj_list,
                                               graph->num_vertices * sizeof(Edge*));
        if (new_adj_list) {
            graph->adj_list = new_adj_list;
        }
    }

    return true;
}

// Add an edge to the graph
bool graph_add_edge(Graph* graph, int src, int dest, double weight) {
    if (!graph_is_valid_vertex(graph, src) || !graph_is_valid_vertex(graph, dest)) {
        fprintf(stderr, "Error: Invalid vertex (src: %d, dest: %d)\n", src, dest);
        return false;
    }

    // For unweighted graphs, weight is always 1.0
    if (!graph->is_weighted) {
        weight = 1.0;
    }

    // Create new edge
    Edge* new_edge = (Edge*)malloc(sizeof(Edge));
    if (!new_edge) {
        fprintf(stderr, "Error: Memory allocation failed for edge\n");
        return false;
    }

    new_edge->dest = dest;
    new_edge->weight = weight;
    new_edge->next = graph->adj_list[src];
    graph->adj_list[src] = new_edge;

    // For undirected graphs, add reverse edge (but not for self-loops)
    if (!graph->is_directed && src != dest) {
        Edge* reverse_edge = (Edge*)malloc(sizeof(Edge));
        if (!reverse_edge) {
            fprintf(stderr, "Error: Memory allocation failed for reverse edge\n");
            return false;
        }
        reverse_edge->dest = src;
        reverse_edge->weight = weight;
        reverse_edge->next = graph->adj_list[dest];
        graph->adj_list[dest] = reverse_edge;
    }

    graph->num_edges++;
    return true;
}

// Remove an edge from the graph
bool graph_remove_edge(Graph* graph, int src, int dest) {
    if (!graph_is_valid_vertex(graph, src) || !graph_is_valid_vertex(graph, dest)) {
        fprintf(stderr, "Error: Invalid vertex (src: %d, dest: %d)\n", src, dest);
        return false;
    }

    bool found = false;

    // Remove edge from src to dest
    Edge** edge_ptr = &graph->adj_list[src];
    while (*edge_ptr) {
        if ((*edge_ptr)->dest == dest) {
            Edge* to_remove = *edge_ptr;
            *edge_ptr = (*edge_ptr)->next;
            free(to_remove);
            found = true;
            break;
        }
        edge_ptr = &(*edge_ptr)->next;
    }

    // For undirected graphs, remove reverse edge (but not for self-loops)
    if (found && !graph->is_directed && src != dest) {
        edge_ptr = &graph->adj_list[dest];
        while (*edge_ptr) {
            if ((*edge_ptr)->dest == src) {
                Edge* to_remove = *edge_ptr;
                *edge_ptr = (*edge_ptr)->next;
                free(to_remove);
                break;
            }
            edge_ptr = &(*edge_ptr)->next;
        }
    }

    if (found) {
        graph->num_edges--;
    }

    return found;
}

// Print graph structure
void graph_print(const Graph* graph) {
    if (!graph) return;

    printf("\n=== Graph Structure ===\n");
    printf("Vertices: %d\n", graph->num_vertices);
    printf("Edges: %d\n", graph->num_edges);
    printf("Type: %s, %s\n",
           graph->is_weighted ? "Weighted" : "Unweighted",
           graph->is_directed ? "Directed" : "Undirected");

    printf("\nAdjacency List:\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("Vertex %d:", i);
        Edge* edge = graph->adj_list[i];
        while (edge) {
            if (graph->is_weighted) {
                printf(" -> %d(%.1f)", edge->dest, edge->weight);
            } else {
                printf(" -> %d", edge->dest);
            }
            edge = edge->next;
        }
        printf("\n");
    }
    printf("=======================\n\n");
}

// Create a new path result
PathResult* path_result_create(void) {
    PathResult* result = (PathResult*)malloc(sizeof(PathResult));
    if (!result) {
        fprintf(stderr, "Error: Memory allocation failed for path result\n");
        return NULL;
    }

    result->path = NULL;
    result->path_length = 0;
    result->total_weight = 0.0;
    result->time_ms = 0.0;
    result->algorithm = NULL;
    result->found = false;

    return result;
}

// Destroy path result and free memory
void path_result_destroy(PathResult* result) {
    if (!result) return;
    free(result->path);
    free(result);
}

// Print path result
void path_result_print(const PathResult* result) {
    if (!result) return;

    printf("  Algorithm: %s\n", result->algorithm ? result->algorithm : "Unknown");
    printf("  Time: %.3f ms\n", result->time_ms);

    if (result->found) {
        printf("  Path: ");
        for (int i = 0; i < result->path_length; i++) {
            printf("%d", result->path[i]);
            if (i < result->path_length - 1) printf(" -> ");
        }
        printf("\n");
        printf("  Total weight: %.1f\n", result->total_weight);
    } else {
        printf("  Path: Not found\n");
    }
}

// Set coordinates for a vertex
bool graph_set_coordinates(Graph* graph, int vertex, double x, double y) {
    if (!graph_is_valid_vertex(graph, vertex)) {
        fprintf(stderr, "Error: Invalid vertex %d\n", vertex);
        return false;
    }

    // Allocate coordinates array if not already allocated
    if (!graph->coords) {
        graph->coords = (Coordinates*)calloc(graph->num_vertices, sizeof(Coordinates));
        if (!graph->coords) {
            fprintf(stderr, "Error: Memory allocation failed for coordinates\n");
            return false;
        }
    }

    graph->coords[vertex].x = x;
    graph->coords[vertex].y = y;
    return true;
}

// Check if graph has coordinates
bool graph_has_coordinates(const Graph* graph) {
    return graph && graph->coords != NULL;
}

// Calculate Euclidean distance between two vertices
double graph_euclidean_distance(const Graph* graph, int v1, int v2) {
    if (!graph_has_coordinates(graph)) {
        return 0.0;
    }
    if (!graph_is_valid_vertex(graph, v1) || !graph_is_valid_vertex(graph, v2)) {
        return 0.0;
    }

    double dx = graph->coords[v2].x - graph->coords[v1].x;
    double dy = graph->coords[v2].y - graph->coords[v1].y;
    return sqrt(dx * dx + dy * dy);
}

// Calculate Manhattan distance between two vertices
double graph_manhattan_distance(const Graph* graph, int v1, int v2) {
    if (!graph_has_coordinates(graph)) {
        return 0.0;
    }
    if (!graph_is_valid_vertex(graph, v1) || !graph_is_valid_vertex(graph, v2)) {
        return 0.0;
    }

    double dx = fabs(graph->coords[v2].x - graph->coords[v1].x);
    double dy = fabs(graph->coords[v2].y - graph->coords[v1].y);
    return dx + dy;
}
