#include "include/graph.h"
#include <stdio.h>

int main() {
    printf("=== Testing Graph with Self-Loops ===\n\n");

    // Test 1: Directed graph with self-loop
    printf("Test 1: Directed Graph with Self-Loop\n");
    Graph* directed = graph_create(3, true, true);
    graph_add_edge(directed, 0, 1, 5.0);
    graph_add_edge(directed, 1, 1, 3.0);  // Self-loop
    graph_add_edge(directed, 1, 2, 2.0);
    graph_print(directed);

    // Test 2: Undirected graph with self-loop
    printf("\nTest 2: Undirected Graph with Self-Loop\n");
    Graph* undirected = graph_create(3, true, false);
    graph_add_edge(undirected, 0, 1, 5.0);
    graph_add_edge(undirected, 1, 1, 3.0);  // Self-loop
    graph_add_edge(undirected, 1, 2, 2.0);
    graph_print(undirected);

    // Test 3: Removing self-loop
    printf("\nTest 3: Removing Self-Loop from Undirected Graph\n");
    graph_remove_edge(undirected, 1, 1);
    graph_print(undirected);

    // Cleanup
    graph_destroy(directed);
    graph_destroy(undirected);

    printf("=== All Tests Completed ===\n");
    return 0;
}
