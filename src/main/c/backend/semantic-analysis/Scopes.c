//
// Created by alexk on 6/20/2025.
//


#include "Scopes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Stack implementation in C by GeeksForGeeks: https://www.geeksforgeeks.org/c/implement-stack-in-c/

// C Program to demonstrate how to Implement a Stack



// Function to initialize the stack with allocation
ScopesStack* initializeScopesStack() {
    ScopesStack* stack = (ScopesStack*)malloc(sizeof(ScopesStack));
    if (stack == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    // Set top index to -1 to indicate an empty stack
    stack->top = -1;
    stack->newScope = 0; // Initialize newScope to 0
    return stack;
}

// Function to check if the stack is empty
int isEmpty(ScopesStack *stack) {
    // If top is -1, the stack is empty
    return stack->top == -1;
}

// Function to check if the stack is full
int isFull(ScopesStack *stack) {
    // If top is MAX_SIZE - 1, the stack is full
    return stack->top == MAX_SIZE - 1;
}

// Function to push an element onto the stack
void push(ScopesStack *stack, int value) {
    // Check for stack overflow
    if (isFull(stack)) {
        printf("ScopesStack Overflow\n");
        return;
    }
    // Increment top and add the value to the top of the stack
    stack->arr[++stack->top] = value;
    printf("Pushed %d onto the stack\n", value);
}

void addNewScope(ScopesStack *stack) {
    // Push the new scope onto the stack
    push(stack, stack->newScope);
    // Increment the newScope counter
    stack->newScope++;

    printf("New scope added: %d\n", stack->newScope);
}

// Function to pop an element from the stack
int popScopesStack(ScopesStack *stack) {
    // Check for stack underflow
    if (isEmpty(stack)) {
        printf("ScopesStack Underflow\n");
        return -1;
    }
    // Return the top element
    int popped = stack->arr[stack->top];
    // decrement top pointer
    stack->top--;
    printf("Popped %d from the stack\n", popped);
    // return the popped element
    return popped;
}

// Function to peek the top element of the stack
int peek(ScopesStack *stack) {
    // Check if the stack is empty
    if (isEmpty(stack)) {
        printf("ScopesStack is empty\n");
        return -1;
    }
    // Return the top element without removing it
    return stack->arr[stack->top];
}

int currentScope(ScopesStack *stack) {
    return peek(stack);
}

// EXTRA: Function to check if a specific element exists in the stack.
// NOTE: Assumes that the stack is ordered in ascending order.
int containsScopesStack(ScopesStack *stack, int value) {
    // Iterate through the stack to check for the value
    int i = 0;
    for (; stack->arr[i] < value && i <= stack->top; i++);
    if (stack->arr[i] == value) {
        printf("found scope %d\n", stack->arr[i]);
        return 1; // Value found
    }
    printf("Not found scope %d\n", stack->arr[i]);
    return 0; // Value not found
}

// Function to free the memory allocated for the scopes stack
void freeScopesStack(ScopesStack* stack) {
    if (stack != NULL) {
        // No need to free the internal array as it's statically allocated
        free(stack);
    }
}
//
// int main() {
//     ScopesStack stack;
//     // Initialize the stack
//     initialize(&stack);
//
//     // Push elements onto the stack and print the stack after each push
//     push(&stack, 3);
//     printf("Top element: %d\n", peek(&stack));
//
//     push(&stack, 5);
//     printf("Top element: %d\n", peek(&stack));
//
//     push(&stack, 2);
//     printf("Top element: %d\n", peek(&stack));
//
//     push(&stack, 8);
//     printf("Top element: %d\n", peek(&stack));
//
//     // Pop elements from the stack and print the stack after each pop
//     while (!isEmpty(&stack)) {
//         printf("Top element: %d\n", peek(&stack));
//         printf("Popped element: %d\n", pop(&stack));
//     }
//
//     return 0;
// }
