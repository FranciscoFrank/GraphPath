#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "astar.h"
#include "bellman_ford.h"

#define MAX_RESULTS 10

// Input graph from user
Graph* input_graph(void) {
    int num_vertices, num_edges;
    bool is_weighted, is_directed;
    char input[10];

    printf("\n=== Graph Creation ===\n");

    // Get number of vertices
    printf("Enter number of vertices: ");
    if (scanf("%d", &num_vertices) != 1 || num_vertices <= 0) {
        fprintf(stderr, "Error: Invalid number of vertices\n");
        return NULL;
    }

    // Get graph type
    printf("Is the graph weighted? (y/n): ");
    if (scanf("%s", input) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return NULL;
    }
    is_weighted = (input[0] == 'y' || input[0] == 'Y');

    printf("Is the graph directed? (y/n): ");
    if (scanf("%s", input) != 1) {
        fprintf(stderr, "Error: Invalid input\n");
        return NULL;
    }
    is_directed = (input[0] == 'y' || input[0] == 'Y');

    Graph* graph = graph_create(num_vertices, is_weighted, is_directed);
    if (!graph) {
        return NULL;
    }

    printf("\nEnter number of edges: ");
    if (scanf("%d", &num_edges) != 1 || num_edges < 0) {
        fprintf(stderr, "Error: Invalid number of edges\n");
        graph_destroy(graph);
        return NULL;
    }

    printf("\n");
    if (is_weighted) {
        printf("Enter edges in format: source destination weight\n");
    } else {
        printf("Enter edges in format: source destination\n");
    }
    printf("(Vertices are numbered from 0 to %d)\n\n", num_vertices - 1);

    // Input edges
    for (int i = 0; i < num_edges; i++) {
        int src, dest;
        double weight = 1.0;

        printf("Edge %d: ", i + 1);

        if (is_weighted) {
            if (scanf("%d %d %lf", &src, &dest, &weight) != 3) {
                fprintf(stderr, "Error: Invalid edge format\n");
                graph_destroy(graph);
                return NULL;
            }
        } else {
            if (scanf("%d %d", &src, &dest) != 2) {
                fprintf(stderr, "Error: Invalid edge format\n");
                graph_destroy(graph);
                return NULL;
            }
        }

        if (!graph_add_edge(graph, src, dest, weight)) {
            fprintf(stderr, "Failed to add edge %d -> %d\n", src, dest);
            graph_destroy(graph);
            return NULL;
        }
    }

    return graph;
}

// Input coordinates for vertices (optional, for A*)
bool input_coordinates(Graph* graph) {
    char input[10];
    printf("\nDo you want to add vertex coordinates for A* algorithm? (y/n): ");
    if (scanf("%s", input) != 1) {
        return false;
    }

    if (input[0] != 'y' && input[0] != 'Y') {
        return false;
    }

    printf("\nEnter coordinates for each vertex (format: x y)\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        double x, y;
        printf("Vertex %d: ", i);
        if (scanf("%lf %lf", &x, &y) != 2) {
            fprintf(stderr, "Error: Invalid coordinates\n");
            return false;
        }
        if (!graph_set_coordinates(graph, i, x, y)) {
            fprintf(stderr, "Error: Failed to set coordinates\n");
            return false;
        }
    }

    printf("\nCoordinates added successfully!\n");
    return true;
}

// Select appropriate algorithms based on graph properties
int select_algorithms(const Graph* graph, PathResult* (*algorithms[])(const Graph*, int, int)) {
    int count = 0;

    if (graph->is_weighted) {
        // For weighted graphs, use all weighted algorithms
        algorithms[count++] = dijkstra_find_path;
        algorithms[count++] = bellman_ford_find_path;

        // Add A* variants
        if (graph_has_coordinates(graph)) {
            algorithms[count++] = astar_euclidean;
            algorithms[count++] = astar_manhattan;
        }
        algorithms[count++] = astar_zero;  // Always include (works without coordinates)
    } else {
        // For unweighted graphs, use BFS and DFS
        algorithms[count++] = bfs_find_path;
        algorithms[count++] = dfs_find_path;
    }

    return count;
}

// Print comprehensive statistics
void print_statistics(const Graph* graph, int start, int end, PathResult** results, int num_results) {
    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    PATHFINDING STATISTICS                  ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    printf("\n--- Graph Information ---\n");
    printf("Type: %s, %s\n",
           graph->is_weighted ? "Weighted" : "Unweighted",
           graph->is_directed ? "Directed" : "Undirected");
    printf("Vertices: %d\n", graph->num_vertices);
    printf("Edges: %d\n", graph->num_edges);
    printf("Search: from vertex %d to vertex %d\n", start, end);

    printf("\n--- Algorithm Results ---\n");
    for (int i = 0; i < num_results; i++) {
        printf("\n[Result %d]\n", i + 1);
        path_result_print(results[i]);
    }

    // Summary comparison
    printf("\n--- Performance Comparison ---\n");
    PathResult* fastest = NULL;
    PathResult* shortest = NULL;

    for (int i = 0; i < num_results; i++) {
        if (results[i]->found) {
            if (!fastest || results[i]->time_ms < fastest->time_ms) {
                fastest = results[i];
            }
            if (!shortest || results[i]->total_weight < shortest->total_weight) {
                shortest = results[i];
            }
        }
    }

    if (fastest) {
        printf("Fastest algorithm: %s (%.3f ms)\n", fastest->algorithm, fastest->time_ms);
    }
    if (shortest && graph->is_weighted) {
        printf("Shortest path found by: %s (weight: %.1f)\n",
               shortest->algorithm, shortest->total_weight);
    }

    printf("\n");
    printf("══════════════════════════════════════════════════════════════\n\n");
}

int main(void) {
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║              GraphPath - Graph Pathfinding Tool            ║\n");
    printf("║                    Version 1.0                             ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n");

    // Input graph
    Graph* graph = input_graph();
    if (!graph) {
        fprintf(stderr, "Failed to create graph\n");
        return 1;
    }

    // Display graph
    graph_print(graph);

    // Optionally input coordinates for A* algorithm
    if (graph->is_weighted) {
        input_coordinates(graph);
    }

    // Input start and end vertices
    int start, end;
    printf("Enter start vertex: ");
    if (scanf("%d", &start) != 1 || !graph_is_valid_vertex(graph, start)) {
        fprintf(stderr, "Error: Invalid start vertex\n");
        graph_destroy(graph);
        return 1;
    }

    printf("Enter end vertex: ");
    if (scanf("%d", &end) != 1 || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid end vertex\n");
        graph_destroy(graph);
        return 1;
    }

    // Select algorithms
    PathResult* (*algorithms[MAX_RESULTS])(const Graph*, int, int);
    int num_algorithms = select_algorithms(graph, algorithms);

    printf("\n--- Running %d algorithm(s)... ---\n", num_algorithms);

    // Run algorithms
    PathResult* results[MAX_RESULTS];
    for (int i = 0; i < num_algorithms; i++) {
        results[i] = algorithms[i](graph, start, end);
        if (!results[i]) {
            fprintf(stderr, "Error: Algorithm %d failed\n", i);
            // Clean up previous results
            for (int j = 0; j < i; j++) {
                path_result_destroy(results[j]);
            }
            graph_destroy(graph);
            return 1;
        }
    }

    // Print statistics
    print_statistics(graph, start, end, results, num_algorithms);

    // Cleanup
    for (int i = 0; i < num_algorithms; i++) {
        path_result_destroy(results[i]);
    }
    graph_destroy(graph);

    return 0;
}
