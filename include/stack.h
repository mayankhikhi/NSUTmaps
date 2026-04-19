#ifndef STACK_H
#define STACK_H

typedef struct Stack {
    int top;
    int capacity;
    int* array;
} Stack;

Stack* createStack(int capacity);
int isFull(Stack* stack);
int isStackEmpty(Stack* stack);
void push(Stack* stack, int item);
int pop(Stack* stack);
int peek(Stack* stack);
void freeStack(Stack* stack);

#endif // STACK_H
