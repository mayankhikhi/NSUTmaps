#include "utils.h"

static void trimNewline(char* s) {
    if (!s) {
        return;
    }
    s[strcspn(s, "\r\n")] = '\0';
}

int readLocations(const char* filename, char* locations[], int maxLocations) {
    FILE* file;
    char buffer[MAX_NAME_LENGTH];
    int count = 0;

    file = fopen(filename, "r");
    if (!file) {
        return -1;
    }

    while (count < maxLocations && fgets(buffer, sizeof(buffer), file)) {
        size_t len;

        trimNewline(buffer);
        len = strlen(buffer);
        if (len == 0) {
            continue;
        }

        locations[count] = (char*)malloc(len + 1);
        if (!locations[count]) {
            fclose(file);
            return -1;
        }

        strcpy(locations[count], buffer);
        count++;
    }

    fclose(file);
    return count;
}

int readEdges(const char* filename, Graph* graph, char* locations[], int locationCount) {
    FILE* file;
    char line[2 * MAX_NAME_LENGTH + 10];

    if (!graph || !locations || locationCount <= 0) {
        return 0;
    }

    file = fopen(filename, "r");
    if (!file) {
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        char from[MAX_NAME_LENGTH];
        char to[MAX_NAME_LENGTH];
        char* comma;
        int foundFrom = -1;
        int foundTo = -1;

        trimNewline(line);
        comma = strchr(line, ',');
        if (!comma) {
            continue;
        }

        *comma = '\0';
        strncpy(from, line, MAX_NAME_LENGTH - 1);
        from[MAX_NAME_LENGTH - 1] = '\0';
        strncpy(to, comma + 1, MAX_NAME_LENGTH - 1);
        to[MAX_NAME_LENGTH - 1] = '\0';

        while (*to == ' ') {
            memmove(to, to + 1, strlen(to));
        }

        for (int i = 0; i < locationCount; i++) {
            if (strcmp(locations[i], from) == 0) {
                foundFrom = i;
            }
            if (strcmp(locations[i], to) == 0) {
                foundTo = i;
            }
        }

        if (foundFrom >= 0 && foundTo >= 0) {
            addEdgeByIndex(graph, foundFrom, foundTo);
        }
    }

    fclose(file);
    return 1;
}

void freeLocations(char* locations[], int count) {
    if (!locations) {
        return;
    }

    for (int i = 0; i < count; i++) {
        free(locations[i]);
        locations[i] = NULL;
    }
}
