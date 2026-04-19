#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "utils.h"
#include "shortest_path.h"

#define MAX_LOCATIONS 100

int main() {
    Graph* graph = createGraph();
    char *locations[MAX_LOCATIONS];
    int locationCount = 0;
    int path[MAX_LOCATIONS];
    int pathLen;

    if (!graph) {
        fprintf(stderr, "Error creating graph.\n");
        return EXIT_FAILURE;
    }

    locationCount = readLocations("data/locations.txt", locations, MAX_LOCATIONS);
    if (locationCount == -1) {
        fprintf(stderr, "Error reading locations.\n");
        freeGraph(graph);
        return EXIT_FAILURE;
    }

    for (int i = 0; i < locationCount; i++) {
        addLocation(graph, locations[i]);
    }

    if (!readEdges("data/edges.txt", graph, locations, locationCount)) {
        fprintf(stderr, "Error reading edges.\n");
        freeLocations(locations, locationCount);
        freeGraph(graph);
        return EXIT_FAILURE;
    }

    displayGraph(graph);

    pathLen = findShortestPath(graph, 0, 1, path, MAX_LOCATIONS);
    if (pathLen > 0) {
        printf("Shortest path from %s to %s: ", graph->locations[0].name, graph->locations[1].name);
        printPath(graph, path, pathLen);
    } else {
        printf("No path found between %s and %s\n", graph->locations[0].name, graph->locations[1].name);
    }

    freeLocations(locations, locationCount);
    freeGraph(graph);

    return EXIT_SUCCESS;
}