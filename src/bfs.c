#include "bfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Simple queue for BFS
typedef struct {
    int* data;
    int front;
    int rear;
    int capacity;
} Queue;

static Queue* queue_create(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) return NULL;

    queue->data = (int*)malloc(capacity * sizeof(int));
    if (!queue->data) {
        free(queue);
        return NULL;
    }

    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

static void queue_destroy(Queue* queue) {
    if (!queue) return;
    free(queue->data);
    free(queue);
}

static bool queue_is_empty(const Queue* queue) {
    return queue->front == queue->rear;
}

static void queue_enqueue(Queue* queue, int value) {
    // Check if queue needs to grow
    if (queue->rear >= queue->capacity) {
        int new_capacity = queue->capacity * 2;
        int* new_data = (int*)realloc(queue->data, new_capacity * sizeof(int));
        if (new_data) {
            queue->data = new_data;
            queue->capacity = new_capacity;
        } else {
            fprintf(stderr, "Warning: Failed to grow queue\n");
            return;  // Skip this enqueue if we can't grow
        }
    }
    queue->data[queue->rear++] = value;
}

static int queue_dequeue(Queue* queue) {
    return queue->data[queue->front++];
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

// BFS algorithm implementation
PathResult* bfs_find_path(const Graph* graph, int start, int end) {
    PathResult* result = path_result_create();
    if (!result) return NULL;

    result->algorithm = "BFS (Breadth-First Search)";

    if (!graph_is_valid_vertex(graph, start) || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid start or end vertex\n");
        return result;
    }

    clock_t start_time = clock();

    int n = graph->num_vertices;
    bool* visited = (bool*)calloc(n, sizeof(bool));
    int* parent = (int*)malloc(n * sizeof(int));
    Queue* queue = queue_create(n);

    if (!visited || !parent || !queue) {
        free(visited);
        free(parent);
        queue_destroy(queue);
        return result;
    }

    // Initialize
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    visited[start] = true;
    queue_enqueue(queue, start);

    // BFS traversal
    while (!queue_is_empty(queue)) {
        int current = queue_dequeue(queue);

        if (current == end) {
            break;
        }

        Edge* edge = graph->adj_list[current];
        while (edge) {
            if (!visited[edge->dest]) {
                visited[edge->dest] = true;
                parent[edge->dest] = current;
                queue_enqueue(queue, edge->dest);
            }
            edge = edge->next;
        }
    }

    // Reconstruct path
    result->path = reconstruct_path(parent, start, end, &result->path_length);
    result->found = (result->path != NULL);

    // Calculate total weight
    if (result->found) {
        result->total_weight = 0.0;
        for (int i = 0; i < result->path_length - 1; i++) {
            int u = result->path[i];
            int v = result->path[i + 1];

            Edge* edge = graph->adj_list[u];
            while (edge && edge->dest != v) {
                edge = edge->next;
            }
            if (edge) {
                result->total_weight += edge->weight;
            }
        }
    }

    clock_t end_time = clock();
    result->time_ms = ((double)(end_time - start_time) / CLOCKS_PER_SEC) * 1000.0;

    free(visited);
    free(parent);
    queue_destroy(queue);

    return result;
}
