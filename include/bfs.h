#ifndef BFS_H
#define BFS_H

#include "graph.h"

// Find path using Breadth-First Search
PathResult* bfs_find_path(const Graph* graph, int start, int end);

#endif // BFS_H
