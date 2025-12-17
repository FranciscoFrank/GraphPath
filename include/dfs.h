#ifndef DFS_H
#define DFS_H

#include "graph.h"

// Find path using Depth-First Search
PathResult* dfs_find_path(const Graph* graph, int start, int end);

#endif // DFS_H
