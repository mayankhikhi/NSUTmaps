#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCATIONS 100
#define MAX_NAME_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
} Location;

typedef struct {
    int adjMatrix[MAX_LOCATIONS][MAX_LOCATIONS];
    int numLocations;
    Location locations[MAX_LOCATIONS];
} Graph;

// Function declarations
Graph* createGraph(void);
void initializeGraph(Graph* graph);
int addLocation(Graph* graph, const char* name);
int findLocationIndex(const Graph* graph, const char* name);
int addEdgeByIndex(Graph* graph, int src, int dest);
int addEdge(Graph* graph, const char* from, const char* to);
void displayGraph(const Graph* graph);
void freeGraph(Graph* graph);

#endif // GRAPH_H