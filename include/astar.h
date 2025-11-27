#ifndef ASTAR_H
#define ASTAR_H

#include "graph.h"

// Heuristic function type
typedef double (*HeuristicFunc)(const Graph*, int, int);

// Find shortest path using A* algorithm with custom heuristic
PathResult* astar_find_path(const Graph* graph, int start, int end,
                           HeuristicFunc heuristic, const char* algorithm_name);

// A* with Euclidean distance heuristic
PathResult* astar_euclidean(const Graph* graph, int start, int end);

// A* with Manhattan distance heuristic
PathResult* astar_manhattan(const Graph* graph, int start, int end);

// A* with zero heuristic (equivalent to Dijkstra)
PathResult* astar_zero(const Graph* graph, int start, int end);

#endif // ASTAR_H
