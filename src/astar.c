#include "astar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <time.h>

// Priority queue node for A*
typedef struct AStarNode {
    int vertex;
    double g_score;  // Cost from start to current vertex
    double f_score;  // g_score + heuristic (estimated total cost)
} AStarNode;

// Priority queue for A* (min-heap based on f_score)
typedef struct {
    AStarNode* nodes;
    int size;
    int capacity;
} AStarPQ;

static AStarPQ* astar_pq_create(int capacity) {
    AStarPQ* pq = (AStarPQ*)malloc(sizeof(AStarPQ));
    if (!pq) return NULL;

    pq->nodes = (AStarNode*)malloc(capacity * sizeof(AStarNode));
    if (!pq->nodes) {
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

static void astar_pq_destroy(AStarPQ* pq) {
    if (!pq) return;
    free(pq->nodes);
    free(pq);
}

static void astar_pq_swap(AStarNode* a, AStarNode* b) {
    AStarNode temp = *a;
    *a = *b;
    *b = temp;
}

static void astar_pq_heapify_up(AStarPQ* pq, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->nodes[index].f_score >= pq->nodes[parent].f_score) {
            break;
        }
        astar_pq_swap(&pq->nodes[index], &pq->nodes[parent]);
        index = parent;
    }
}

static void astar_pq_heapify_down(AStarPQ* pq, int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < pq->size && pq->nodes[left].f_score < pq->nodes[smallest].f_score) {
            smallest = left;
        }
        if (right < pq->size && pq->nodes[right].f_score < pq->nodes[smallest].f_score) {
            smallest = right;
        }

        if (smallest == index) {
            break;
        }

        astar_pq_swap(&pq->nodes[index], &pq->nodes[smallest]);
        index = smallest;
    }
}

static void astar_pq_push(AStarPQ* pq, int vertex, double g_score, double f_score) {
    // Check if PQ needs to grow
    if (pq->size >= pq->capacity) {
        int new_capacity = pq->capacity * 2;
        AStarNode* new_nodes = (AStarNode*)realloc(pq->nodes, new_capacity * sizeof(AStarNode));
        if (new_nodes) {
            pq->nodes = new_nodes;
            pq->capacity = new_capacity;
        } else {
            fprintf(stderr, "Warning: Failed to grow A* priority queue\n");
            return;  // Skip this push if we can't grow
        }
    }

    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].g_score = g_score;
    pq->nodes[pq->size].f_score = f_score;
    astar_pq_heapify_up(pq, pq->size);
    pq->size++;
}

static AStarNode astar_pq_pop(AStarPQ* pq) {
    AStarNode min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    astar_pq_heapify_down(pq, 0);
    return min;
}

static bool astar_pq_is_empty(const AStarPQ* pq) {
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

// A* algorithm implementation with custom heuristic
PathResult* astar_find_path(const Graph* graph, int start, int end,
                           HeuristicFunc heuristic, const char* algorithm_name) {
    PathResult* result = path_result_create();
    if (!result) return NULL;

    result->algorithm = algorithm_name;

    if (!graph_is_valid_vertex(graph, start) || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid start or end vertex\n");
        return result;
    }

    clock_t start_time = clock();

    int n = graph->num_vertices;
    double* g_score = (double*)malloc(n * sizeof(double));
    double* f_score = (double*)malloc(n * sizeof(double));
    int* parent = (int*)malloc(n * sizeof(int));
    bool* in_closed_set = (bool*)calloc(n, sizeof(bool));
    // Start with reasonable initial capacity, will grow dynamically if needed
    AStarPQ* open_set = astar_pq_create(n > 1000 ? n : 1000);

    if (!g_score || !f_score || !parent || !in_closed_set || !open_set) {
        free(g_score);
        free(f_score);
        free(parent);
        free(in_closed_set);
        astar_pq_destroy(open_set);
        return result;
    }

    // Initialize scores
    for (int i = 0; i < n; i++) {
        g_score[i] = DBL_MAX;
        f_score[i] = DBL_MAX;
        parent[i] = -1;
    }

    g_score[start] = 0.0;
    f_score[start] = heuristic(graph, start, end);
    astar_pq_push(open_set, start, g_score[start], f_score[start]);

    // A* main loop
    while (!astar_pq_is_empty(open_set)) {
        AStarNode current = astar_pq_pop(open_set);
        int u = current.vertex;

        if (in_closed_set[u]) continue;
        in_closed_set[u] = true;

        // Found the goal
        if (u == end) break;

        // Explore neighbors
        Edge* edge = graph->adj_list[u];
        while (edge) {
            int v = edge->dest;
            double weight = edge->weight;

            if (in_closed_set[v]) {
                edge = edge->next;
                continue;
            }

            double tentative_g_score = g_score[u] + weight;

            if (tentative_g_score < g_score[v]) {
                parent[v] = u;
                g_score[v] = tentative_g_score;
                f_score[v] = g_score[v] + heuristic(graph, v, end);
                astar_pq_push(open_set, v, g_score[v], f_score[v]);
            }

            edge = edge->next;
        }
    }

    // Reconstruct path
    result->path = reconstruct_path(parent, start, end, &result->path_length);
    result->found = (result->path != NULL);
    result->total_weight = (result->found) ? g_score[end] : 0.0;

    clock_t end_time = clock();
    result->time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    free(g_score);
    free(f_score);
    free(parent);
    free(in_closed_set);
    astar_pq_destroy(open_set);

    return result;
}

// Zero heuristic (always returns 0)
static double zero_heuristic(const Graph* graph, int v1, int v2) {
    (void)graph;  // Unused
    (void)v1;     // Unused
    (void)v2;     // Unused
    return 0.0;
}

// A* with Euclidean distance heuristic
PathResult* astar_euclidean(const Graph* graph, int start, int end) {
    if (!graph_has_coordinates(graph)) {
        fprintf(stderr, "Warning: Graph has no coordinates, using zero heuristic\n");
        return astar_find_path(graph, start, end, zero_heuristic, "A* (Euclidean-fallback)");
    }
    return astar_find_path(graph, start, end, graph_euclidean_distance, "A* (Euclidean)");
}

// A* with Manhattan distance heuristic
PathResult* astar_manhattan(const Graph* graph, int start, int end) {
    if (!graph_has_coordinates(graph)) {
        fprintf(stderr, "Warning: Graph has no coordinates, using zero heuristic\n");
        return astar_find_path(graph, start, end, zero_heuristic, "A* (Manhattan-fallback)");
    }
    return astar_find_path(graph, start, end, graph_manhattan_distance, "A* (Manhattan)");
}

// A* with zero heuristic (equivalent to Dijkstra)
PathResult* astar_zero(const Graph* graph, int start, int end) {
    return astar_find_path(graph, start, end, zero_heuristic, "A* (Zero/Dijkstra-like)");
}
