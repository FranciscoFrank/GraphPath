#include "dfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stack for iterative DFS
typedef struct {
    int* data;
    int top;
    int capacity;
} Stack;

static Stack* stack_create(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;

    stack->data = (int*)malloc(capacity * sizeof(int));
    if (!stack->data) {
        free(stack);
        return NULL;
    }

    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

static void stack_destroy(Stack* stack) {
    if (!stack) return;
    free(stack->data);
    free(stack);
}

static bool stack_is_empty(const Stack* stack) {
    return stack->top == -1;
}

static void stack_push(Stack* stack, int value) {
    // Check if stack needs to grow
    if (stack->top + 1 >= stack->capacity) {
        int new_capacity = stack->capacity * 2;
        int* new_data = (int*)realloc(stack->data, new_capacity * sizeof(int));
        if (new_data) {
            stack->data = new_data;
            stack->capacity = new_capacity;
        } else {
            fprintf(stderr, "Warning: Failed to grow stack\n");
            return;  // Skip this push if we can't grow
        }
    }
    stack->data[++stack->top] = value;
}

static int stack_pop(Stack* stack) {
    return stack->data[stack->top--];
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

// DFS algorithm implementation (iterative)
PathResult* dfs_find_path(const Graph* graph, int start, int end) {
    PathResult* result = path_result_create();
    if (!result) return NULL;

    result->algorithm = "DFS (Depth-First Search)";

    if (!graph_is_valid_vertex(graph, start) || !graph_is_valid_vertex(graph, end)) {
        fprintf(stderr, "Error: Invalid start or end vertex\n");
        return result;
    }

    clock_t start_time = clock();

    int n = graph->num_vertices;
    bool* visited = (bool*)calloc(n, sizeof(bool));
    int* parent = (int*)malloc(n * sizeof(int));
    Stack* stack = stack_create(n);

    if (!visited || !parent || !stack) {
        free(visited);
        free(parent);
        stack_destroy(stack);
        return result;
    }

    // Initialize
    for (int i = 0; i < n; i++) {
        parent[i] = -1;
    }

    stack_push(stack, start);

    // DFS traversal
    while (!stack_is_empty(stack)) {
        int current = stack_pop(stack);

        if (!visited[current]) {
            visited[current] = true;

            if (current == end) {
                break;
            }

            Edge* edge = graph->adj_list[current];
            while (edge) {
                if (!visited[edge->dest]) {
                    if (parent[edge->dest] == -1 && edge->dest != start) {
                        parent[edge->dest] = current;
                    }
                    stack_push(stack, edge->dest);
                }
                edge = edge->next;
            }
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
    stack_destroy(stack);

    return result;
}
