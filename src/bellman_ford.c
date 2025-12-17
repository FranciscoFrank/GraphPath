#include "bellman_ford.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>

// Edge list structure for Bellman-Ford
typedef struct EdgeList {
    int src;
    int dest;
    double weight;
} EdgeList;

// Reconstruct path from parent array
static int* reconstruct_path(int* parent, int start, int end, int* path_length) {
    if (parent[end] == -1 && start != end) {
        *path_length = 0;
        return NULL;
    }

    // Count path length
    int count = 0;
    for (int v = end; v != -1; v = parent[v]) {
        count++;
    }

    int* path = (int*)malloc(count * sizeof(int));
    if (!path) return NULL;

    // Fill path in reverse
    int index = count - 1;
    for (int v = end; v != -1; v = parent[v]) {
        path[index--] = v;
    }

    *path_length = count;
    return path;
}

// Bellman-Ford algorithm implementation
PathResult* bellman_ford_find_path(const Graph* graph, int start, int end) {
    PathResult* result = path_result_create();
    if (!result) return NULL;

    result->algorithm = "Bellman-Ford";

    if (!graph_is_valid_vertex(graph, start) || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid start or end vertex\n");
        return result;
    }

    clock_t start_time = clock();

    int n = graph->num_vertices;

    // Build edge list from adjacency list
    int edge_count = 0;
    for (int i = 0; i < n; i++) {
        Edge* edge = graph->adj_list[i];
        while (edge) {
            edge_count++;
            edge = edge->next;
        }
    }

    EdgeList* edges = (EdgeList*)malloc(edge_count * sizeof(EdgeList));
    if (!edges) {
        fprintf(stderr, "Error: Memory allocation failed for edge list\n");
        return result;
    }

    int edge_index = 0;
    for (int i = 0; i < n; i++) {
        Edge* edge = graph->adj_list[i];
        while (edge) {
            edges[edge_index].src = i;
            edges[edge_index].dest = edge->dest;
            edges[edge_index].weight = edge->weight;
            edge_index++;
            edge = edge->next;
        }
    }

    double* dist = (double*)malloc(n * sizeof(double));
    int* parent = (int*)malloc(n * sizeof(int));

    if (!dist || !parent) {
        free(edges);
        free(dist);
        free(parent);
        return result;
    }

    // Initialize distances
    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;
        parent[i] = -1;
    }
    dist[start] = 0.0;

    // Relax edges (n-1) times
    for (int i = 0; i < n - 1; i++) {
        bool updated = false;
        for (int j = 0; j < edge_count; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            double weight = edges[j].weight;

            if (dist[u] != DBL_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                updated = true;
            }
        }
        // Early termination if no updates
        if (!updated) break;
    }

    // Reconstruct the path first
    result->path = reconstruct_path(parent, start, end, &result->path_length);
    result->found = (result->path != NULL);
    result->total_weight = (result->found) ? dist[end] : 0.0;

    // Check if there's a negative cycle that affects the shortest path
    // We only care about negative cycles that can improve the distance to vertices on our path
    if (result->found) {
        bool path_affected = false;

        // Check if any vertex on the path can still be improved (is in a negative cycle)
        for (int i = 0; i < result->path_length; i++) {
            int vertex = result->path[i];

            // Try one more relaxation for edges leading to this vertex
            for (int j = 0; j < edge_count; j++) {
                int u = edges[j].src;
                int v = edges[j].dest;
                double weight = edges[j].weight;

                if (v == vertex && dist[u] != DBL_MAX && dist[u] + weight < dist[v]) {
                    path_affected = true;
                    break;
                }
            }

            if (path_affected) break;
        }

        if (path_affected) {
            fprintf(stderr, "Warning: Path is affected by negative weight cycle!\n");
            free(result->path);
            result->path = NULL;
            result->path_length = 0;
            result->found = false;
            result->total_weight = 0.0;
        }
    }

    clock_t end_time = clock();
    result->time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    free(edges);
    free(dist);
    free(parent);

    return result;
}
