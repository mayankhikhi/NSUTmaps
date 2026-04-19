#include <stdlib.h>
#include "queue.h"

Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    if (!queue) {
        return NULL;
    }
    queue->front = NULL;
    queue->rear = NULL;
    queue->size = 0;
    return queue;
}

bool isQueueEmpty(Queue* queue) {
    return !queue || queue->size == 0;
}

void enqueue(Queue* queue, int location) {
    QueueNode* newNode;

    if (!queue) {
        return;
    }

    newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        return;
    }

    newNode->location = location;
    newNode->next = NULL;

    if (queue->rear) {
        queue->rear->next = newNode;
    } else {
        queue->front = newNode;
    }

    queue->rear = newNode;
    queue->size++;
}

int dequeue(Queue* queue) {
    QueueNode* node;
    int location;

    if (isQueueEmpty(queue)) {
        return -1;
    }

    node = queue->front;
    location = node->location;
    queue->front = node->next;

    if (!queue->front) {
        queue->rear = NULL;
    }

    free(node);
    queue->size--;
    return location;
}

int front(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return -1;
    }
    return queue->front->location;
}

void freeQueue(Queue* queue) {
    if (!queue) {
        return;
    }

    while (!isQueueEmpty(queue)) {
        dequeue(queue);
    }

    free(queue);
}
