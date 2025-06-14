#include "symbolTable.h"
// #include "./../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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


// Linked LIst Fuente: https://www.geeksforgeeks.org/c-program-to-implement-singly-linked-list/


// Function to create a new node 
SymbolEntryNode* createSymbol(const SymbolEntry data) {
    SymbolEntryNode* newNode = malloc(sizeof(SymbolEntryNode));
    newNode->entry = data;
    newNode->next = NULL;
    return newNode;
}

// Function to create the table
SymbolTable *createSymbolTable() {
    return calloc(1, sizeof(SymbolTable)); // Initialize the head of the linked list to NULL
}

// Function to insert a new element at the beginning of the singly linked list
void insertSymbol(SymbolTable* symbolTable, const SymbolEntry data) {
    SymbolEntryNode* newNode = createSymbol(data);
    newNode->entry.identifier = strdup(data.identifier); // Duplicate string
    newNode->next = symbolTable->head;
    symbolTable->head = newNode;
}

// Function to print an element
void printSymbolEntry(const SymbolEntry entry) {
    switch (entry.type) {
        case INTEGER_TYPE:
            printf("[(int) %s: {%d}]", entry.identifier, entry.value.integerData);
            break;
        case FLOAT_TYPE:
            printf("[(float) %s: {%.2f}]", entry.identifier, entry.value.floatData);
            break;
        case VECTOR_TYPE:
            printf("[(vector) %s: {%.2f, %.2f}]",
                entry.identifier,
                entry.value.vectorData.x,
                entry.value.vectorData.y);
            break;
        case FUNCTION_TYPE:
            printf("[(function) %s: {ret: %d, params: ...}]", entry.identifier, entry.value.functionData.returnType);
            break;
        case ARRAY_TYPE:
            printf("[(array) %s: {type: %d, elems: ...}]", entry.identifier, entry.value.arrayData.dataType);
            break;
        default:
            printf("[(unknown) %s]", entry.identifier);
            break;
    }
}

// Function to print the LinkedList
void printSymbolTable(SymbolTable* symbolTable) {
    const SymbolEntryNode* head = symbolTable->head; // symbolTable is just another way of referring to the head of the linked list

    const SymbolEntryNode* temp = head;
    while (temp != NULL) {
        printSymbolEntry(temp->entry);
        printf(" -> ");
        temp = temp->next;
    }
    printf("NULL\n");
}
// Function to free the linked list
void freeSymbolTable(SymbolTable* symbolTable) {
    SymbolEntryNode* head = symbolTable->head;
    while (head != NULL) {
        SymbolEntryNode* temp = head;
        head = head->next;

        if (temp->entry.identifier != NULL) {
            free(temp->entry.identifier);
        }

        // Free dynamically allocated arrays in FUNCTION_TYPE and ARRAY_TYPE
        if (temp->entry.type == FUNCTION_TYPE && temp->entry.value.functionData.dataTypes != NULL) {
            free(temp->entry.value.functionData.dataTypes);
        }
        if (temp->entry.type == ARRAY_TYPE && temp->entry.value.arrayData.elements != NULL) {
            free(temp->entry.value.arrayData.elements);
        }

        free(temp);
    }
    free(symbolTable); // Free the symbol table itself
}

// Function to get a symbol entry by identifier
SymbolEntry getSymbolEntry(SymbolTable* symbolTable, const char* identifier) {
    SymbolEntryNode* head = symbolTable->head;
    
    while (head != NULL) {
        if (strcmp(head->entry.identifier, identifier) == 0) {
            return head->entry; // Return the found entry
        }
        head = head->next;
    }

    SymbolEntry emptyEntry = {0}; // Initialize an empty SymbolEntry
    emptyEntry.identifier = NULL;
    emptyEntry.type = NULL_TYPE;
    // Initialize other fields of emptyEntry as needed
    return emptyEntry;
}

// Function to check if a symbol exists in the table
int symbolExists(SymbolTable* symbolTable, const char* identifier) {
    SymbolEntryNode* head = symbolTable->head;

    while (head != NULL) {
        if (strcmp(head->entry.identifier, identifier) == 0) {
            return 1; // Symbol exists
        }
        head = head->next;
    }

    return 0; // Symbol does not exist
}

// Function to set all default functions
void setDefaultFunctions(SymbolTable* symbolTable) {
    SymbolType* circleDataTypes = malloc(3 * sizeof(SymbolType));
    circleDataTypes[0] = VECTOR_TYPE;
    circleDataTypes[1] = FLOAT_TYPE;
    circleDataTypes[2] = NULL_TYPE; // Null-terminated array of data types
    const SymbolEntry circleFunction = {
        .identifier = "circle",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .dataTypes = circleDataTypes,
            .hasInfiniteParameters = 0,
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, circleFunction);

    SymbolType* curveDataTypes = malloc(2 * sizeof(SymbolType));
    curveDataTypes[0] = VECTOR_TYPE;
    curveDataTypes[1] = NULL_TYPE; // Null-terminated array of data types
    const SymbolEntry curveFunction = {
        .identifier = "curve",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .dataTypes = curveDataTypes,
            .hasInfiniteParameters = 1, // Indicates that the function can take an infinite number of parameters;
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, curveFunction);

    SymbolType* fillDataTypes = malloc(2 * sizeof(SymbolType));
    fillDataTypes[0] = INTEGER_TYPE; // TODO: replace with HEX data type
    fillDataTypes[1] = NULL_TYPE; // Null-terminated array of data types
    const SymbolEntry fillFunction = {
        .identifier = "fill",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .dataTypes = fillDataTypes,
            .hasInfiniteParameters = 0,
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, fillFunction);

    SymbolType* strokeDataTypes = malloc(2 * sizeof(SymbolType));
    strokeDataTypes[0] = INTEGER_TYPE;  // TODO: replace with HEX data type
    strokeDataTypes[1] = NULL_TYPE; // Null-terminated array of data types
    const SymbolEntry strokeFunction = {
        .identifier = "stroke",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .dataTypes = strokeDataTypes,
            .hasInfiniteParameters = 0,
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, strokeFunction);

}


SymbolType typeOfExpressionList(ExpressionList* expr_list) {
    if (!expr_list) return INVALID_TYPE;
    // TODO
}

SymbolType typeOfFactor(Factor *factor) {
    return INVALID_TYPE; // TODO: Implement this function
}

static SymbolType typeOfArray(Array* array) {
    if (!array) return INVALID_TYPE;
    
    SymbolType leftType, rightType = INVALID_TYPE;
    
    switch (array->type) {
        case INTERVAL_ARRAY:
            leftType = typeOfExpression(array->leftExpression);
            rightType = typeOfExpression(array->rightExpression);
            if(leftType == INTEGER_TYPE && rightType == INTEGER_TYPE) {
                return ARRAY_TYPE;
            } else {
                return INVALID_TYPE; // Incompatible types
            }
            break;
        case IDENTIFIER_ARRAY:
            return getSymbolEntry(currentCompilerState()->symbolTable, array->identifier).type; 
            break;
        case BASIC_ARRAY:
            if (array->expressionList) {
                return typeOfExpressionList(array->expressionList);
            }
            break;
    }
    return INVALID_TYPE;
}

SymbolType typeOfExpression(Expression* expression) {
    if (!expression) return INVALID_TYPE;
    
    SymbolType resultType1, resultType2 = INVALID_TYPE;
    switch (expression->type) {
        case ADDITION:
        case SUBTRACTION:
        case MULTIPLICATION:
        case DIVISION:
        case MODULUS:
            resultType1 = typeOfExpression(expression->leftExpression);
            resultType2 = typeOfExpression(expression->rightExpression);

            if(resultType1 == INTEGER_TYPE && resultType2 == INTEGER_TYPE) {
                return INTEGER_TYPE;
            } else if(resultType1 == FLOAT_TYPE || resultType2 == FLOAT_TYPE) {
                return FLOAT_TYPE;
            } 
            return INVALID_TYPE; // Incompatible types
            
            break;
        case GET_X:
        case GET_Y:
            resultType1 = typeOfExpression(expression->expression);
            if (resultType1 != VECTOR_TYPE) {
                return INVALID_TYPE; 
            } 
            return VECTOR_TYPE; // Return the type of the vector

            break;
        case FACTOR:
            return typeOfFactor(expression->factor);
            break;
        case ARRAY_ACCESS:
            resultType1 = typeOfArray(expression->array);
            resultType2 = typeOfExpression(expression->indexExpression);
            if (resultType2 != INTEGER_TYPE || resultType1 != ARRAY_TYPE) {
                return INVALID_TYPE; // Index must be an integer
            }

            return ARRAY_TYPE; // Return the type of the array

            break;
        case FUNCTION_EXPRESSION:
            if (expression->functionArguments) {
                // TODO chequear argumentos
            }

            SymbolEntry functionEntry = getSymbolEntry(currentCompilerState()->symbolTable, expression->functionIdentifier);
            return functionEntry.value.functionData.returnType; // Return the type of the function
   
            break;
    }
    return INVALID_TYPE;
}
// Example usage of the symbol table
/*
int main() {
    SymbolTable symbolTable = createSymbolTable();

    // Create and insert symbols
    const SymbolEntry entry1 = {"x", INTEGER_TYPE, .value.integerData = 10};
    insertSymbol(&symbolTable, entry1);

    const SymbolEntry entry2 = {"y", FLOAT_TYPE, .value.floatData = 20.5};
    insertSymbol(&symbolTable, entry2);

    const SymbolEntry entry3 = {"v", VECTOR_TYPE, .value.vectorData = {1.0, 2.0}};
    insertSymbol(&symbolTable, entry3);

    // Print the symbol table
    printSymbolTable(symbolTable);

    // Free the symbol table
    freeSymbolTable(symbolTable);

    return 0;
}
*/

// Uncomment to test the example usage
/*
    struct Node* head = NULL;
    insertSymbol(&head, 10);
*/