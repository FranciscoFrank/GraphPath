#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    free(graph);
}

// Check if vertex is valid
bool graph_is_valid_vertex(const Graph* graph, int vertex) {
    return graph && vertex >= 0 && vertex < graph->num_vertices;
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

    // For undirected graphs, add reverse edge
    if (!graph->is_directed) {
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
