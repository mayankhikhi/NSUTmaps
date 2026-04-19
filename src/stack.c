#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int*)malloc(stack->capacity * sizeof(int));
    return stack;
}

int isFull(Stack* stack) {
    return stack->top == stack->capacity - 1;
}

int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, int item) {
    if (isFull(stack)) {
        return;
    }
    stack->array[++stack->top] = item;
}

int pop(Stack* stack) {
    if (isStackEmpty(stack)) {
        return -1; // Return -1 if stack is empty
    }
    return stack->array[stack->top--];
}

int peek(Stack* stack) {
    if (isStackEmpty(stack)) {
        return -1; // Return -1 if stack is empty
    }
    return stack->array[stack->top];
}

void freeStack(Stack* stack) {
    free(stack->array);
    free(stack);
}
