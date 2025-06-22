//
// Created by alexk on 6/20/2025.
//

#ifndef SCOPES_H
#define SCOPES_H


// Define the maximim capacity of the stack
#define MAX_SCOPE_DEPTH 2048

// Define a structure for the stack
typedef struct {
    // Array to store stack elements
    int arr[MAX_SCOPE_DEPTH];
    // Index of the top element in the stack
    int top;
    int newScope;
} ScopesStack;

// functions exclusive to scopes
ScopesStack* initializeScopesStack();
void addNewScope(ScopesStack *stack);
int currentScope(ScopesStack *stack);
int getNextScope(ScopesStack* stack);

// generic stack functions
int popScopesStack(ScopesStack *stack);
int containsScopesStack(ScopesStack *stack, int value);
void freeScopesStack(ScopesStack* stack);

#endif //SCOPES_H
