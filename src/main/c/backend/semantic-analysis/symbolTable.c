#include "symbolTable.h"
// #include "./../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdio.h>
#include <stdlib.h>
/*
// Hash function (by Daniel J. Bernstein)
unsigned long hash(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % TABLE_SIZE;
}
*/



// // C Program for Implementation of Singly Linked List



// Fuente: https://www.geeksforgeeks.org/c-program-to-implement-singly-linked-list/


struct SymbolEntry {
    char* identifier; // Identifier name
    SymbolType type; // Type of the identifier
    union {
        int integerData; // Integer value for integer identifiers
        float floatData; // Float value for float identifiers
        struct {
            float x; // X component for vector identifiers
            float y; // Y component for vector identifiers
        } vectorData; // Vector details
        struct {
            SymbolType* dataTypes; // Data types for the function parameters (Null Terminated). Of course, we shouldn't use FUNCTION_TYPE as a parameter type
            SymbolType returnType; // Type of the identifier
            void (*functionPointer)(); // Pointer to the function implementation
        } functionData; // Function details
    } value; // Value associated with the identifier
    
};

// Define the Node structure
struct SymbolEntryNode {
    struct SymbolEntry entry;
    struct Node* next;
};

// Function to create a new node 
struct Node* createSymbol(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a new element at the beginning of the singly linked list
void insertSymbol(struct Node** head, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}


// Function to print the LinkedList
void printSymbolTable(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

/*
    struct Node* head = NULL;
    insertSymbol(&head, 10);
*/