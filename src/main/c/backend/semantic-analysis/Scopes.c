#include "Scopes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


ScopesStack* initializeScopesStack() {
    ScopesStack* stack = (ScopesStack*)malloc(sizeof(ScopesStack));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    // Set top index to -1 to indicate an empty stack
    stack->top = -1;
    stack->newScope = 0; 
    return stack;
}

int isEmpty(ScopesStack *stack) {
    return stack->top == -1;
}

int isFull(ScopesStack *stack) {
    return stack->top == MAX_SCOPE_DEPTH - 1;
}

void pushScope(ScopesStack *stack, int value) {
    if (isFull(stack)) {     // Check for stack overflow
        printf("ScopesStack Overflow\n");
        return;
    }

    stack->arr[++stack->top] = value;
}

void addNewScope(ScopesStack *stack) {
    pushScope(stack, stack->newScope);
    stack->newScope++;
}

int popScopesStack(ScopesStack *stack) {
    if (isEmpty(stack)) { // Check for stack underflow
        printf("ScopesStack Underflow\n");
        return -1;
    }

    int popped = stack->arr[stack->top];
    stack->top--;

    return popped;
}

int peek(ScopesStack *stack) {
    if (isEmpty(stack)) {
        printf("ScopesStack is empty\n");
        return -1;
    }

    return stack->arr[stack->top];
}

int currentScope(ScopesStack *stack) {
    return peek(stack);
}

// EXTRA: Function to check if a specific element exists in the stack.
// NOTE: Assumes that the stack is ordered in ascending order.
int containsScopesStack(ScopesStack *stack, int value) {
    int i = 0;
    for (; stack->arr[i] < value && i < stack->top; i++);
    if (stack->arr[i] == value) {
        return 1; // Value found
    }
    return 0; // Value not found
}

void freeScopesStack(ScopesStack* stack) {
    if (stack != NULL) {
        // No need to free the internal array as it's statically allocated
        free(stack);
    }
}

int getNextScope(ScopesStack* stack) {
    return stack->newScope;
}
