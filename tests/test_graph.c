#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "graph.h"
#include "bfs.h"
#include "dfs.h"
#include "dijkstra.h"

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

int main(void) {
    printf("\nTPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPW\n");
    printf("Q                  GraphPath Test Suite                     Q\n");
    printf("ZPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP]\n\n");

    test_graph_creation();
    test_edge_addition();
    test_bfs_simple_path();
    test_dfs_simple_path();
    test_dijkstra_weighted();
    test_no_path();
    test_self_loop();

    printf("\n");
    printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n");
    printf("All tests passed! \n");
    printf("PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPP\n\n");

    return 0;
}
