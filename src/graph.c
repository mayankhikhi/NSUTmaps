#include "graph.h"

static void clearGraph(Graph* graph) {
    graph->numLocations = 0;
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        graph->locations[i].name[0] = '\0';
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph->adjMatrix[i][j] = 0;
        }
    }
}

Graph* createGraph(void) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if (!graph) {
        return NULL;
    }
    clearGraph(graph);
    return graph;
}

void initializeGraph(Graph* graph) {
    if (!graph) {
        return;
    }
    clearGraph(graph);
}

int findLocationIndex(const Graph* graph, const char* name) {
    if (!graph || !name) {
        return -1;
    }

    for (int i = 0; i < graph->numLocations; i++) {
        if (strcmp(graph->locations[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

int addLocation(Graph* graph, const char* name) {
    int existing;

    if (!graph || !name) {
        return -1;
    }

    existing = findLocationIndex(graph, name);
    if (existing >= 0) {
        return existing;
    }

    if (graph->numLocations >= MAX_LOCATIONS) {
        return -1;
    }

    strncpy(graph->locations[graph->numLocations].name, name, MAX_NAME_LENGTH - 1);
    graph->locations[graph->numLocations].name[MAX_NAME_LENGTH - 1] = '\0';
    graph->numLocations++;

    return graph->numLocations - 1;
}

int addEdgeByIndex(Graph* graph, int src, int dest) {
    if (!graph || src < 0 || dest < 0 || src >= graph->numLocations || dest >= graph->numLocations) {
        return 0;
    }

    graph->adjMatrix[src][dest] = 1;
    graph->adjMatrix[dest][src] = 1;
    return 1;
}

int addEdge(Graph* graph, const char* from, const char* to) {
    int src = findLocationIndex(graph, from);
    int dest = findLocationIndex(graph, to);

    if (src < 0 || dest < 0) {
        return 0;
    }

    return addEdgeByIndex(graph, src, dest);
}

void displayGraph(const Graph* graph) {
    if (!graph) {
        return;
    }

    for (int i = 0; i < graph->numLocations; i++) {
        printf("%s: ", graph->locations[i].name);
        for (int j = 0; j < graph->numLocations; j++) {
            if (graph->adjMatrix[i][j]) {
                printf("%s ", graph->locations[j].name);
            }
        }
        printf("\n");
    }
}

void freeGraph(Graph* graph) {
    free(graph);
}
