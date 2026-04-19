#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

int readLocations(const char* filename, char* locations[], int maxLocations);
int readEdges(const char* filename, Graph* graph, char* locations[], int locationCount);
void freeLocations(char* locations[], int count);

#endif // UTILS_H