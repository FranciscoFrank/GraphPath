#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>

// Priority queue node
typedef struct PQNode {
    int vertex;
    double distance;
} PQNode;

// Simple priority queue (min-heap)
typedef struct {
    PQNode* nodes;
    int size;
    int capacity;
} PriorityQueue;

static PriorityQueue* pq_create(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) return NULL;

    pq->nodes = (PQNode*)malloc(capacity * sizeof(PQNode));
    if (!pq->nodes) {
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

static void pq_destroy(PriorityQueue* pq) {
    if (!pq) return;
    free(pq->nodes);
    free(pq);
}

static void pq_swap(PQNode* a, PQNode* b) {
    PQNode temp = *a;
    *a = *b;
    *b = temp;
}

static void pq_heapify_up(PriorityQueue* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->nodes[index].distance >= pq->nodes[parent].distance) {
            break;
        }
        pq_swap(&pq->nodes[index], &pq->nodes[parent]);
        index = parent;
    }
}

static void pq_heapify_down(PriorityQueue* pq, int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < pq->size && pq->nodes[left].distance < pq->nodes[smallest].distance) {
            smallest = left;
        }
        if (right < pq->size && pq->nodes[right].distance < pq->nodes[smallest].distance) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        pq_swap(&pq->nodes[index], &pq->nodes[smallest]);
        index = smallest;
    }
}

static void pq_push(PriorityQueue* pq, int vertex, double distance) {
    if (pq->size >= pq->capacity) return;

    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].distance = distance;
    pq_heapify_up(pq, pq->size);
    pq->size++;
}

static PQNode pq_pop(PriorityQueue* pq) {
    PQNode min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    pq_heapify_down(pq, 0);
    return min;
}

static bool pq_is_empty(const PriorityQueue* pq) {
    return pq->size == 0;
}

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

// Dijkstra's algorithm implementation
PathResult* dijkstra_find_path(const Graph* graph, int start, int end) {
    PathResult* result = path_result_create();
    if (!result) return NULL;

    result->algorithm = "Dijkstra";

    if (!graph_is_valid_vertex(graph, start) || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid start or end vertex\n");
        return result;
    }

    clock_t start_time = clock();

    int n = graph->num_vertices;
    double* dist = (double*)malloc(n * sizeof(double));
    int* parent = (int*)malloc(n * sizeof(int));
    bool* visited = (bool*)calloc(n, sizeof(bool));
    PriorityQueue* pq = pq_create(n * n);

    if (!dist || !parent || !visited || !pq) {
        free(dist);
        free(parent);
        free(visited);
        pq_destroy(pq);
        return result;
    }

    // Initialize distances
    for (int i = 0; i < n; i++) {
        dist[i] = DBL_MAX;
        parent[i] = -1;
    }

    dist[start] = 0.0;
    pq_push(pq, start, 0.0);

    // Dijkstra's main loop
    while (!pq_is_empty(pq)) {
        PQNode current = pq_pop(pq);
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = true;

        if (u == end) break;

        Edge* edge = graph->adj_list[u];
        while (edge) {
            int v = edge->dest;
            double weight = edge->weight;

            if (!visited[v] && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq_push(pq, v, dist[v]);
            }
            edge = edge->next;
        }
    }

    // Reconstruct path
    result->path = reconstruct_path(parent, start, end, &result->path_length);
    result->found = (result->path != NULL);
    result->total_weight = (result->found) ? dist[end] : 0.0;

    clock_t end_time = clock();
    result->time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    free(dist);
    free(parent);
    free(visited);
    pq_destroy(pq);

    return result;
}
