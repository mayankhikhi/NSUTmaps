#ifndef SHORTEST_PATH_H
#define SHORTEST_PATH_H

#include "graph.h"

int findShortestPath(const Graph* graph, int startVertex, int endVertex, int path[], int maxPathLen);
void printPath(const Graph* graph, const int path[], int pathLen);

// Backward-compatible wrappers
void dijkstra(Graph* graph, int startVertex, int endVertex);
void bfs_shortest_path(Graph* graph, int startVertex, int endVertex);

#endif // SHORTEST_PATH_H