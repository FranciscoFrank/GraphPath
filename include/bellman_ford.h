#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include "graph.h"

// Find shortest path using Bellman-Ford algorithm
// Can handle negative weights and detect negative cycles
PathResult* bellman_ford_find_path(const Graph* graph, int start, int end);

#endif // BELLMAN_FORD_H
