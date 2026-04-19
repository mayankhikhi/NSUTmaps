#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void test_graph_creation() {
    Graph* graph = createGraph();
    addLocation(graph, "A");
    addLocation(graph, "B");
    addLocation(graph, "C");
    addEdgeByIndex(graph, 0, 1);
    addEdgeByIndex(graph, 1, 2);
    addEdgeByIndex(graph, 2, 0);

    printf("Graph created successfully.\n");
    displayGraph(graph);
    freeGraph(graph);
}

void test_add_edge() {
    Graph* graph = createGraph();
    addLocation(graph, "A");
    addLocation(graph, "B");
    addEdgeByIndex(graph, 0, 1);
    
    if (graph->adjMatrix[0][1] == 1 && graph->adjMatrix[1][0] == 1) {
        printf("Edge added successfully between 0 and 1.\n");
    } else {
        printf("Failed to add edge between 0 and 1.\n");
    }

    freeGraph(graph);
}

void test_display_graph() {
    Graph* graph = createGraph();
    addLocation(graph, "A");
    addLocation(graph, "B");
    addLocation(graph, "C");
    addEdgeByIndex(graph, 0, 1);
    addEdgeByIndex(graph, 1, 2);
    
    printf("Displaying graph:\n");
    displayGraph(graph);
    
    freeGraph(graph);
}

int main() {
    test_graph_creation();
    test_add_edge();
    test_display_graph();
    
    return 0;
}