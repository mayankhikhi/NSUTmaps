#include <stdio.h>
#include <string.h>
#include "shortest_path.h"
#include "queue.h"

int findShortestPath(const Graph* graph, int startVertex, int endVertex, int path[], int maxPathLen) {
    int visited[MAX_LOCATIONS] = {0};
    int parent[MAX_LOCATIONS];
    Queue* queue;
    int pathLen = 0;
    int cur;

    if (!graph || !path || maxPathLen <= 0) {
        return -1;
    }

    if (startVertex < 0 || endVertex < 0 || startVertex >= graph->numLocations || endVertex >= graph->numLocations) {
        return -1;
    }

    for (int i = 0; i < MAX_LOCATIONS; i++) {
        parent[i] = -1;
    }

    queue = createQueue();
    if (!queue) {
        return -1;
    }

    visited[startVertex] = 1;
    enqueue(queue, startVertex);

    while (!isQueueEmpty(queue)) {
        int node = dequeue(queue);

        if (node == endVertex) {
            break;
        }

        for (int i = 0; i < graph->numLocations; i++) {
            if (graph->adjMatrix[node][i] && !visited[i]) {
                visited[i] = 1;
                parent[i] = node;
                enqueue(queue, i);
            }
        }
    }

    if (!visited[endVertex]) {
        freeQueue(queue);
        return -1;
    }

    cur = endVertex;
    while (cur != -1 && pathLen < maxPathLen) {
        path[pathLen++] = cur;
        cur = parent[cur];
    }

    for (int i = 0; i < pathLen / 2; i++) {
        int tmp = path[i];
        path[i] = path[pathLen - i - 1];
        path[pathLen - i - 1] = tmp;
    }

    freeQueue(queue);
    return pathLen;
}

void printPath(const Graph* graph, const int path[], int pathLen) {
    if (!graph || !path || pathLen <= 0) {
        printf("No path found.\n");
        return;
    }

    for (int i = 0; i < pathLen; i++) {
        printf("%s", graph->locations[path[i]].name);
        if (i + 1 < pathLen) {
            printf(" -> ");
        }
    }
    printf("\n");
}

void dijkstra(Graph* graph, int startVertex, int endVertex) {
    int path[MAX_LOCATIONS];
    int pathLen = findShortestPath(graph, startVertex, endVertex, path, MAX_LOCATIONS);

    if (pathLen < 0) {
        printf("No path found between %d and %d\n", startVertex, endVertex);
        return;
    }

    printf("Shortest path (unweighted BFS) from %d to %d: ", startVertex, endVertex);
    printPath(graph, path, pathLen);
}

void bfs_shortest_path(Graph* graph, int startVertex, int endVertex) {
    dijkstra(graph, startVertex, endVertex);
}
