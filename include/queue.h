#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

typedef struct QueueNode {
    int location; // Index of the location in the graph
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// Function to create a new queue
Queue* createQueue();

// Function to check if the queue is empty
bool isQueueEmpty(Queue* queue);

// Function to enqueue an element
void enqueue(Queue* queue, int location);

// Function to dequeue an element
int dequeue(Queue* queue);

// Function to get the front element of the queue
int front(Queue* queue);

// Function to free the queue
void freeQueue(Queue* queue);

#endif // QUEUE_H
