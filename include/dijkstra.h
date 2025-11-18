#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "graph.h"

// Find shortest path using Dijkstra's algorithm
PathResult* dijkstra_find_path(const Graph* graph, int start, int end);

#endif // DIJKSTRA_H
