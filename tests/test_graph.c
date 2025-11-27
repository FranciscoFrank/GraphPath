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

    test_graph_creation();
    test_edge_addition();
    test_bfs_simple_path();
    test_dfs_simple_path();
    test_dijkstra_weighted();
    test_no_path();
    test_self_loop();
    test_coordinates();
    test_astar_euclidean();
    test_astar_manhattan();
    test_astar_zero();
    test_bellman_ford_basic();

    printf("\n");
    printf("╔════════════════════════════════════════════════════════════╗\n");
    printf("║                    All tests passed ✓!                     ║\n");
    printf("╚════════════════════════════════════════════════════════════╝\n\n");

    return 0;
}
