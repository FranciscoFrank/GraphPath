#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"
#include "astar.h"
#include "bellman_ford.h"

#define TEST_PASSED printf(" ")
#define TEST_FAILED printf(" ")

// Test graph creation
void test_graph_creation(void) {
    printf("Testing graph creation... ");

    Graph* graph = graph_create(5, false, false);
    assert(graph != NULL);
    assert(graph->num_vertices == 5);
    assert(graph->num_edges == 0);
    assert(graph->is_weighted == false);
    assert(graph->is_directed == false);

    graph_destroy(graph);
    TEST_PASSED;
    printf("Graph creation test passed\n");
}

// Test edge addition
void test_edge_addition(void) {
    printf("Testing edge addition... ");

    Graph* graph = graph_create(4, false, false);
    assert(graph != NULL);

    assert(graph_add_edge(graph, 0, 1, 1.0) == true);
    assert(graph_add_edge(graph, 1, 2, 1.0) == true);
    assert(graph->num_edges == 2);

    // Invalid edges
    assert(graph_add_edge(graph, -1, 1, 1.0) == false);
    assert(graph_add_edge(graph, 0, 10, 1.0) == false);

    graph_destroy(graph);
    TEST_PASSED;
    printf("Edge addition test passed\n");
}

// Test BFS on simple path
void test_bfs_simple_path(void) {
    printf("Testing BFS simple path... ");

    Graph* graph = graph_create(4, false, false);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 1, 2, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);

    PathResult* result = bfs_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 4);
    assert(result->path[0] == 0);
    assert(result->path[3] == 3);
    assert(result->total_weight == 3.0);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("BFS simple path test passed\n");
}

// Test DFS on simple path
void test_dfs_simple_path(void) {
    printf("Testing DFS simple path... ");

    Graph* graph = graph_create(4, false, false);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 1, 2, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);

    PathResult* result = dfs_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length > 0);
    assert(result->path[0] == 0);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("DFS simple path test passed\n");
}

// Test Dijkstra on weighted graph
void test_dijkstra_weighted(void) {
    printf("Testing Dijkstra on weighted graph... ");

    Graph* graph = graph_create(5, true, true);
    graph_add_edge(graph, 0, 1, 4.0);
    graph_add_edge(graph, 0, 2, 1.0);
    graph_add_edge(graph, 2, 1, 2.0);
    graph_add_edge(graph, 1, 3, 1.0);
    graph_add_edge(graph, 2, 3, 5.0);

    PathResult* result = dijkstra_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->total_weight == 4.0); // 0->2->1->3

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Dijkstra weighted graph test passed\n");
}

// Test no path scenario
void test_no_path(void) {
    printf("Testing no path scenario... ");

    Graph* graph = graph_create(4, false, true);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);

    PathResult* result = bfs_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == false);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("No path scenario test passed\n");
}

// Test self-loop
void test_self_loop(void) {
    printf("Testing self-loop... ");

    Graph* graph = graph_create(3, false, false);
    graph_add_edge(graph, 0, 0, 1.0);

    PathResult* result = bfs_find_path(graph, 0, 0);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 1);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Self-loop test passed\n");
}

// Test self-loop in directed graph
void test_self_loop_directed(void) {
    printf("Testing self-loop in directed graph... ");

    Graph* graph = graph_create(3, true, true);
    graph_add_edge(graph, 0, 1, 5.0);
    graph_add_edge(graph, 1, 1, 3.0);  // Self-loop
    graph_add_edge(graph, 1, 2, 2.0);

    // Verify self-loop exists
    PathResult* result = dijkstra_find_path(graph, 1, 1);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 1);
    assert(result->total_weight == 0.0);  // Self-loop shouldn't be traversed for same start/end

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Self-loop in directed graph test passed\n");
}

// Test self-loop in undirected graph
void test_self_loop_undirected(void) {
    printf("Testing self-loop in undirected graph... ");

    Graph* graph = graph_create(3, true, false);
    graph_add_edge(graph, 0, 1, 5.0);
    graph_add_edge(graph, 1, 1, 3.0);  // Self-loop
    graph_add_edge(graph, 1, 2, 2.0);

    PathResult* result = dijkstra_find_path(graph, 0, 2);
    assert(result != NULL);
    assert(result->found == true);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Self-loop in undirected graph test passed\n");
}

// Test removing self-loop
void test_remove_self_loop(void) {
    printf("Testing removing self-loop... ");

    Graph* graph = graph_create(3, true, false);
    graph_add_edge(graph, 0, 1, 5.0);
    graph_add_edge(graph, 1, 1, 3.0);  // Self-loop
    graph_add_edge(graph, 1, 2, 2.0);

    int initial_edges = graph->num_edges;
    graph_remove_edge(graph, 1, 1);
    assert(graph->num_edges == initial_edges - 1);

    graph_destroy(graph);
    TEST_PASSED;
    printf("Removing self-loop test passed\n");
}

// Test edge removal
void test_edge_removal(void) {
    printf("Testing edge removal... ");

    Graph* graph = graph_create(4, false, false);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 1, 2, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);

    assert(graph->num_edges == 3);
    graph_remove_edge(graph, 1, 2);
    assert(graph->num_edges == 2);

    // Path should not exist after removal
    PathResult* result = bfs_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == false);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Edge removal test passed\n");
}

// Test cycle detection scenario
void test_cycle_graph(void) {
    printf("Testing graph with cycle... ");

    Graph* graph = graph_create(4, false, false);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 1, 2, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);
    graph_add_edge(graph, 3, 0, 1.0);  // Creates a cycle

    PathResult* result = bfs_find_path(graph, 0, 2);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 3);  // 0->1->2

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Graph with cycle test passed\n");
}

// Test weighted vs unweighted graph
void test_weighted_vs_unweighted(void) {
    printf("Testing weighted vs unweighted graph... ");

    // Unweighted graph
    Graph* unweighted = graph_create(3, false, false);
    graph_add_edge(unweighted, 0, 1, 5.0);  // Weight ignored
    graph_add_edge(unweighted, 1, 2, 10.0); // Weight ignored

    PathResult* result1 = bfs_find_path(unweighted, 0, 2);
    assert(result1 != NULL);
    assert(result1->found == true);

    // Weighted graph
    Graph* weighted = graph_create(4, true, false);
    graph_add_edge(weighted, 0, 1, 1.0);
    graph_add_edge(weighted, 1, 3, 1.0);
    graph_add_edge(weighted, 0, 2, 5.0);
    graph_add_edge(weighted, 2, 3, 1.0);

    PathResult* result2 = dijkstra_find_path(weighted, 0, 3);
    assert(result2 != NULL);
    assert(result2->found == true);
    assert(result2->total_weight == 2.0);  // Should take 0->1->3

    path_result_destroy(result1);
    path_result_destroy(result2);
    graph_destroy(unweighted);
    graph_destroy(weighted);
    TEST_PASSED;
    printf("Weighted vs unweighted graph test passed\n");
}

// Test directed vs undirected edges
void test_directed_vs_undirected(void) {
    printf("Testing directed vs undirected edges... ");

    // Directed graph
    Graph* directed = graph_create(3, false, true);
    graph_add_edge(directed, 0, 1, 1.0);
    graph_add_edge(directed, 1, 2, 1.0);

    PathResult* result1 = bfs_find_path(directed, 0, 2);
    assert(result1->found == true);

    PathResult* result2 = bfs_find_path(directed, 2, 0);
    assert(result2->found == false);  // No reverse path

    // Undirected graph
    Graph* undirected = graph_create(3, false, false);
    graph_add_edge(undirected, 0, 1, 1.0);
    graph_add_edge(undirected, 1, 2, 1.0);

    PathResult* result3 = bfs_find_path(undirected, 0, 2);
    assert(result3->found == true);

    PathResult* result4 = bfs_find_path(undirected, 2, 0);
    assert(result4->found == true);  // Reverse path exists

    path_result_destroy(result1);
    path_result_destroy(result2);
    path_result_destroy(result3);
    path_result_destroy(result4);
    graph_destroy(directed);
    graph_destroy(undirected);
    TEST_PASSED;
    printf("Directed vs undirected edges test passed\n");
}

// Test large graph performance
void test_large_graph(void) {
    printf("Testing large graph... ");

    Graph* graph = graph_create(100, true, false);

    // Create a path through all vertices
    for (int i = 0; i < 99; i++) {
        graph_add_edge(graph, i, i + 1, 1.0);
    }

    PathResult* result = dijkstra_find_path(graph, 0, 99);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 100);
    assert(result->total_weight == 99.0);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Large graph test passed\n");
}

// Test single vertex graph
void test_single_vertex(void) {
    printf("Testing single vertex graph... ");

    Graph* graph = graph_create(1, false, false);

    PathResult* result = bfs_find_path(graph, 0, 0);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length == 1);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Single vertex graph test passed\n");
}

// Test invalid vertex indices
void test_invalid_vertices(void) {
    printf("Testing invalid vertex indices... ");

    Graph* graph = graph_create(5, false, false);
    graph_add_edge(graph, 0, 1, 1.0);

    // Test with invalid source
    PathResult* result1 = bfs_find_path(graph, -1, 1);
    assert(result1 != NULL);
    assert(result1->found == false);

    // Test with invalid destination
    PathResult* result2 = bfs_find_path(graph, 0, 10);
    assert(result2 != NULL);
    assert(result2->found == false);

    path_result_destroy(result1);
    path_result_destroy(result2);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Invalid vertex indices test passed\n");
}

// Test Bellman-Ford with negative weights (if supported)
void test_bellman_ford_negative_weights(void) {
    printf("Testing Bellman-Ford with negative weights... ");

    Graph* graph = graph_create(4, true, true);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 1, 2, -2.0);  // Negative weight
    graph_add_edge(graph, 2, 3, 1.0);
    graph_add_edge(graph, 0, 3, 5.0);

    PathResult* result = bellman_ford_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    // Should take path 0->1->2->3 with weight 0.0 instead of 0->3 with weight 5.0
    assert(result->total_weight == 0.0);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Bellman-Ford with negative weights test passed\n");
}

// Test A* with Euclidean heuristic
void test_astar_euclidean(void) {
    printf("Testing A* with Euclidean heuristic... ");

    Graph* graph = graph_create(5, true, false);
    graph_add_edge(graph, 0, 1, 4.0);
    graph_add_edge(graph, 0, 2, 1.0);
    graph_add_edge(graph, 2, 1, 2.0);
    graph_add_edge(graph, 1, 3, 1.0);
    graph_add_edge(graph, 2, 3, 5.0);
    graph_add_edge(graph, 3, 4, 3.0);

    // Set coordinates for vertices (in a line)
    graph_set_coordinates(graph, 0, 0.0, 0.0);
    graph_set_coordinates(graph, 1, 3.0, 0.0);
    graph_set_coordinates(graph, 2, 1.0, 0.0);
    graph_set_coordinates(graph, 3, 4.0, 0.0);
    graph_set_coordinates(graph, 4, 7.0, 0.0);

    PathResult* result = astar_euclidean(graph, 0, 4);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->path_length > 0);
    assert(result->path[0] == 0);
    assert(result->path[result->path_length - 1] == 4);

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("A* Euclidean test passed\n");
}

// Test A* with Manhattan heuristic
void test_astar_manhattan(void) {
    printf("Testing A* with Manhattan heuristic... ");

    Graph* graph = graph_create(4, true, false);
    graph_add_edge(graph, 0, 1, 1.0);
    graph_add_edge(graph, 0, 2, 4.0);
    graph_add_edge(graph, 1, 3, 1.0);
    graph_add_edge(graph, 2, 3, 1.0);

    // Set coordinates (grid layout)
    graph_set_coordinates(graph, 0, 0.0, 0.0);
    graph_set_coordinates(graph, 1, 1.0, 0.0);
    graph_set_coordinates(graph, 2, 0.0, 1.0);
    graph_set_coordinates(graph, 3, 1.0, 1.0);

    PathResult* result = astar_manhattan(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->total_weight == 2.0); // 0->1->3

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("A* Manhattan test passed\n");
}

// Test A* with zero heuristic (should behave like Dijkstra)
void test_astar_zero(void) {
    printf("Testing A* with zero heuristic... ");

    Graph* graph = graph_create(5, true, true);
    graph_add_edge(graph, 0, 1, 4.0);
    graph_add_edge(graph, 0, 2, 1.0);
    graph_add_edge(graph, 2, 1, 2.0);
    graph_add_edge(graph, 1, 3, 1.0);
    graph_add_edge(graph, 2, 3, 5.0);

    PathResult* result = astar_zero(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->total_weight == 4.0); // 0->2->1->3

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("A* zero heuristic test passed\n");
}

// Test Bellman-Ford on weighted graph
void test_bellman_ford_basic(void) {
    printf("Testing Bellman-Ford basic... ");

    Graph* graph = graph_create(5, true, true);
    graph_add_edge(graph, 0, 1, 4.0);
    graph_add_edge(graph, 0, 2, 1.0);
    graph_add_edge(graph, 2, 1, 2.0);
    graph_add_edge(graph, 1, 3, 1.0);
    graph_add_edge(graph, 2, 3, 5.0);

    PathResult* result = bellman_ford_find_path(graph, 0, 3);
    assert(result != NULL);
    assert(result->found == true);
    assert(result->total_weight == 4.0); // 0->2->1->3

    path_result_destroy(result);
    graph_destroy(graph);
    TEST_PASSED;
    printf("Bellman-Ford basic test passed\n");
}

// Test coordinate functions
void test_coordinates(void) {
    printf("Testing coordinate functions... ");

    Graph* graph = graph_create(3, true, false);

    assert(graph_has_coordinates(graph) == false);

    assert(graph_set_coordinates(graph, 0, 0.0, 0.0) == true);
    assert(graph_set_coordinates(graph, 1, 3.0, 4.0) == true);
    assert(graph_set_coordinates(graph, 2, 6.0, 8.0) == true);

    assert(graph_has_coordinates(graph) == true);

    // Test Euclidean distance: (0,0) to (3,4) = 5.0
    double dist = graph_euclidean_distance(graph, 0, 1);
    assert(fabs(dist - 5.0) < 0.01);

    // Test Manhattan distance: (0,0) to (3,4) = 7.0
    double manhattan = graph_manhattan_distance(graph, 0, 1);
    assert(fabs(manhattan - 7.0) < 0.01);

    graph_destroy(graph);
    TEST_PASSED;
    printf("Coordinate functions test passed\n");
}

int main(void) {
    printf("\n╔═════════════════════════════════════════════════════════════╗\n");
    printf("║                  GraphPath Test Suite                       ║\n");
    printf("╚═════════════════════════════════════════════════════════════╝\n\n");

    printf("--- Basic Graph Operations ---\n");
    test_graph_creation();
    test_edge_addition();
    test_edge_removal();
    test_single_vertex();
    test_invalid_vertices();

    printf("\n--- Graph Types ---\n");
    test_weighted_vs_unweighted();
    test_directed_vs_undirected();

    printf("\n--- Self-Loops ---\n");
    test_self_loop();
    test_self_loop_directed();
    test_self_loop_undirected();
    test_remove_self_loop();

    printf("\n--- Path Finding Algorithms ---\n");
    test_bfs_simple_path();
    test_dfs_simple_path();
    test_dijkstra_weighted();
    test_no_path();
    test_cycle_graph();

    printf("\n--- Coordinate-Based Algorithms ---\n");
    test_coordinates();
    test_astar_euclidean();
    test_astar_manhattan();
    test_astar_zero();

    printf("\n--- Bellman-Ford Algorithm ---\n");
    test_bellman_ford_basic();
    test_bellman_ford_negative_weights();

    printf("\n--- Performance Tests ---\n");
    test_large_graph();

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    All tests passed ✓!                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    return 0;
}
