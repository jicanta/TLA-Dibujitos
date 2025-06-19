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

const char *symbolTypeToString(SymbolType type) {
    switch (type) {
        case INTEGER_TYPE: return "INTEGER_TYPE";
        case FLOAT_TYPE: return "FLOAT_TYPE";
        case VECTOR_TYPE: return "VECTOR_TYPE";
        case FUNCTION_TYPE: return "FUNCTION_TYPE";
        case ARRAY_TYPE: return "ARRAY_TYPE";
        case NULL_TYPE: return "NULL_TYPE";
        default: return "UNKNOWN_TYPE";
    }
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
        if (temp->entry.type == FUNCTION_TYPE && temp->entry.value.functionData.parameterType != NULL) {
            free(temp->entry.value.functionData.parameterType);
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
    emptyEntry.type = NULL_TYPE; // Set to an invalid type
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

void setDefaultColors(SymbolTable* symbolTable) {
    SymbolEntry redColor = {
        .identifier = "RED",
        .type = INTEGER_TYPE,
        .value.integerData = 0xFF0000 // Hexadecimal representation of red color
    };
    insertSymbol(symbolTable, redColor);

    SymbolEntry greenColor = {
        .identifier = "GREEN",
        .type = INTEGER_TYPE,
        .value.integerData = 0x00FF00 // Hexadecimal representation of green color
    };
    insertSymbol(symbolTable, greenColor);

    SymbolEntry blueColor = {
        .identifier = "BLUE",
        .type = INTEGER_TYPE,
        .value.integerData = 0x0000FF // Hexadecimal representation of blue color
    };
    insertSymbol(symbolTable, blueColor);

    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "YELLOW",
        .type = INTEGER_TYPE,
        .value.integerData = 0xFFFF00 // Hexadecimal representation of yellow color
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "BLACK",
        .type = INTEGER_TYPE,
        .value.integerData = 0x000000 // Hexadecimal representation of black color
    });
    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "INVISIBLE",
        .type = INTEGER_TYPE,
        .value.integerData = 0x000000 // Hexadecimal representation of black color
    });
}
// Function to set all default functions
void setDefaultFunctions(SymbolTable* symbolTable) {
    setDefaultColors(symbolTable);
    SymbolType* circleDataTypes = malloc(2 * sizeof(SymbolType));
    circleDataTypes[0] = VECTOR_TYPE;
    circleDataTypes[1] = FLOAT_TYPE;
    const SymbolEntry circleFunction = {
        .identifier = "circle",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = circleDataTypes,
            .parameterCount = 2,
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, circleFunction);

    SymbolType* curveDataTypes = malloc(1 * sizeof(SymbolType));
    curveDataTypes[0] = VECTOR_TYPE;
    const SymbolEntry curveFunction = {
        .identifier = "curve",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = curveDataTypes,
            .parameterCount = -1, // Indicates that the function can take an infinite number of parameters;
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, curveFunction);

    SymbolType* fillDataTypes = malloc(1 * sizeof(SymbolType));
    fillDataTypes[0] = INTEGER_TYPE; // TODO: replace with HEX data type
    const SymbolEntry fillFunction = {
        .identifier = "fill",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = fillDataTypes,
            .parameterCount = 1,
            .returnType = NULL_TYPE,
            .functionPointer = NULL // Set to the actual function pointer later
        }
    };
    insertSymbol(symbolTable, fillFunction);

    SymbolType* strokeDataTypes = malloc(1 * sizeof(SymbolType));
    strokeDataTypes[0] = INTEGER_TYPE;  // TODO: replace with HEX data type
    const SymbolEntry strokeFunction = {
        .identifier = "stroke",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = strokeDataTypes,
            .parameterCount = 1,
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
    SymbolType leftType, rightType = INVALID_TYPE;
    if(factor == NULL) return INVALID_TYPE; // Check for NULL factor
    switch (factor->type) {
        case IDENTIFIER_FACTOR:
            return getSymbolEntry(currentCompilerState()->symbolTable, factor->identifier).type;
            break;
        case INTEGER_FACTOR:
            return INTEGER_TYPE;
            break;
        case DECIMAL_FACTOR:
            return FLOAT_TYPE;
            break;
        case PARENTHESIS_FACTOR:
            return typeOfExpression(factor->expression);
            break;
        case VECTOR_FACTOR:
            leftType = typeOfExpression(factor->vector->x);
            rightType = typeOfExpression(factor->vector->y);
            if(leftType == rightType) {
                return VECTOR_TYPE; // Both expressions in the vector have the same type
            }
            break;
    }
    return INVALID_TYPE;
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
            else if(resultType1 == VECTOR_TYPE && resultType2 == VECTOR_TYPE) {
                return VECTOR_TYPE; // Both expressions are vectors
            } 
            return INVALID_TYPE; // Incompatible types
            
            break;
        case GET_X:
        case GET_Y:
            resultType1 = typeOfExpression(expression->expression);
            if (resultType1 != VECTOR_TYPE) {
                return INVALID_TYPE; 
            } 
            return FLOAT_TYPE; // TODO: Return the type of the vector

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

int intValueFactor(Factor* factor) {
    switch (factor->type)  // Check the type of the factor
    {
    case IDENTIFIER_FACTOR:
        return getSymbolEntry(currentCompilerState()->symbolTable, factor->identifier).value.integerData;
    case INTEGER_FACTOR:
        return factor->integerExpression;
    case DECIMAL_FACTOR:
        return (int)factor->floatExpression; // Convert float to int
    case PARENTHESIS_FACTOR:
        return intValueExpression(factor->expression);
    case VECTOR_FACTOR:
        // Assuming the vector has x and y as integer values
        // return (int)factor->vector->x + (int)factor->vector->y; // Example: sum of x and y
    default:
        break;
    }
    return 0; // Default case if no valid factor is found
}

int intValueExpression(Expression* expression) {
    switch (expression->type)  // Check the type of the factor
    {
    case ADDITION:
        return intValueExpression(expression->leftExpression) + intValueExpression(expression->rightExpression);
    case SUBTRACTION:
        return intValueExpression(expression->leftExpression) - intValueExpression(expression->rightExpression);
    case MULTIPLICATION:
        return intValueExpression(expression->leftExpression) * intValueExpression(expression->rightExpression);
    case DIVISION: // Note: Division by zero should be handled elsewhere
        return intValueExpression(expression->leftExpression) / intValueExpression(expression->rightExpression);
    case MODULUS:
        return intValueExpression(expression->leftExpression) % intValueExpression(expression->rightExpression);
    case FACTOR:
        return intValueFactor(expression->factor);
    case GET_X:
        return intValueExpression(expression->factor->vector->x);
    case GET_Y:
        return intValueExpression(expression->factor->vector->y);
    case ARRAY_ACCESS:
        // Assuming the array is of type INTEGER_TYPE and the index is valid
    case FUNCTION_EXPRESSION:
        // Assuming the function returns an integer value
        break;
    default:
        break;
    }
    return 0;
}

float floatValueFactor(Factor* factor) {
    switch (factor->type)  // Check the type of the factor
    {
    case IDENTIFIER_FACTOR:
        return getSymbolEntry(currentCompilerState()->symbolTable, factor->identifier).value.floatData;
    case INTEGER_FACTOR:
        return (float)factor->integerExpression; // Convert int to float
    case DECIMAL_FACTOR:
        return factor->floatExpression;
    case PARENTHESIS_FACTOR:
        return floatValueExpression(factor->expression);
    case VECTOR_FACTOR:
    default:
        break;
    }
    return 0.0f; // Default case if no valid factor is found
}

float floatValueExpression(Expression* expression) {
    switch (expression->type)  // Check the type of the factor
    {
    case ADDITION:
        return floatValueExpression(expression->leftExpression) + floatValueExpression(expression->rightExpression);
    case SUBTRACTION:
        return floatValueExpression(expression->leftExpression) - floatValueExpression(expression->rightExpression);
    case MULTIPLICATION:
        return floatValueExpression(expression->leftExpression) * floatValueExpression(expression->rightExpression);
    case DIVISION: // Note: Division by zero should be handled elsewhere
        return floatValueExpression(expression->leftExpression) / floatValueExpression(expression->rightExpression);
    case FACTOR:
        return floatValueFactor(expression->factor);
    case GET_X:
        return floatValueExpression(expression->factor->vector->x);
    case GET_Y:
        return floatValueExpression(expression->factor->vector->y); 
    case ARRAY_ACCESS:
        int index = intValueExpression(expression->indexExpression);
        Expressions* arrayElements = expression->array->expressionList->expressions;
        while(index-- && arrayElements != NULL) {
            arrayElements = arrayElements->next; // Move to the next element in the list
        } // TODO
        return floatValueExpression(arrayElements->expression);  

    case FUNCTION_EXPRESSION:
        // Assuming the function returns a float value
        break;
    default:
        break;
    }
    return 0.0f;
}

VectorData vectorValueFactor(Factor* factor) {
    VectorData vectorData = {0.0f, 0.0f}; // Initialize to zero
    switch (factor->type)  // Check the type of the factor
    {
    case IDENTIFIER_FACTOR:
        return getSymbolEntry(currentCompilerState()->symbolTable, factor->identifier).value.vectorData;
        break;
    case INTEGER_TYPE:
        vectorData.x = intValueExpression(factor->vector->x); // Convert int to float
        vectorData.y = intValueExpression(factor->vector->y); // Assuming both x and y are the same for integer factor
        break;
    case FLOAT_TYPE:
        vectorData.x = floatValueExpression(factor->vector->x); // Convert int to float
        vectorData.y = floatValueExpression(factor->vector->y); // Assuming both x and y are the same for integer factor
        break;
    default:
        break;
    }

    return vectorData;
}

VectorData vectorValueExpression(Expression *expression) {
    VectorData vectorData = {0.0f, 0.0f}; // Initialize to zero
    VectorData v1, v2; // Temporary variables for vector operations
    switch (expression->type)  // Check the type of the factor
    {
    case ADDITION:
        v1 = vectorValueExpression(expression->leftExpression);
        v2 = vectorValueExpression(expression->rightExpression);
        vectorData.x = v1.x + v2.x;
        vectorData.y = v1.y + v2.y;
        break;
    case SUBTRACTION:
        v1 = vectorValueExpression(expression->leftExpression);
        v2 = vectorValueExpression(expression->rightExpression);
        vectorData.x = v1.x - v2.x;
        vectorData.y = v1.y - v2.y;
        break;
    case MULTIPLICATION:
        v1 = vectorValueExpression(expression->leftExpression);
        v2 = vectorValueExpression(expression->rightExpression);
        // TODO
        break;
    case DIVISION: 
        // TODO ERROR
        break;
    case FACTOR:
        vectorData = vectorValueFactor(expression->factor);
        break;
    case GET_X:
        // vectorData.x = floatValueFactor(expression->factor->vector->x);
        break;
    case GET_Y:
        // vectorData.y = floatValueFactor(expression->factor->vector->y); 
        break;
    case ARRAY_ACCESS:
        // Assuming the array is of type VECTOR_TYPE and the index is valid
        int index = intValueExpression(expression->indexExpression);
        Expressions* arrayElements = expression->array->expressionList->expressions;
        while(index-- && arrayElements != NULL) {
            arrayElements = arrayElements->next; // Move to the next element in the list
        } // TODO
        return vectorValueExpression(arrayElements->expression);  
    case FUNCTION_EXPRESSION:
        // Assuming the function returns a VectorData value
        break;
    default:
        break;
    }
    return vectorData;
}

char *stringValue(StringPartList *list) {
    if (!list) return NULL; // Check for NULL list
    size_t length = 0;
    StringPartList *current = list;
    
    // Calculate the total length of the string
    while (current) {
        switch (current->stringPart->type) 
        {
        case STRING_SEGMENT:
            length += strlen(current->stringPart->string); 
            break;
        case IDENTIFIER_SEGMENT:
            SymbolEntry identifierEntry = getSymbolEntry(currentCompilerState()->symbolTable, current->stringPart->identifier); 
            switch (identifierEntry.type)
            {
            case INTEGER_TYPE:
                length += snprintf(NULL, 0, "%d", identifierEntry.value.integerData); 
                break;
            case FLOAT_TYPE:
                length += snprintf(NULL, 0, "%f", identifierEntry.value.floatData);
            default:
                break;
            }
            break;
        default:
            break;
        }
        current = current->next;
    }
    
    char *result = calloc(length + 1, sizeof(char)); // +1 for the null terminator
    if (!result) return NULL; // Check for memory allocation failure
    
    current = list;
    
    // Concatenate all parts into the result string
    char *resultPtr = result;
    while (current) {
        switch (current->stringPart->type) 
        {
        case STRING_SEGMENT:
            for(int i = 0; current->stringPart->string[i]; i++) {
                *resultPtr++ = current->stringPart->string[i]; // Copy the string segment
            }

            break;
        case IDENTIFIER_SEGMENT:
            SymbolEntry identifierEntry = getSymbolEntry(currentCompilerState()->symbolTable, current->stringPart->identifier); 
            switch (identifierEntry.type)
            {
            case INTEGER_TYPE:
                resultPtr += sprintf(resultPtr, "%d", identifierEntry.value.integerData); 
                break;
            case FLOAT_TYPE:
                resultPtr += sprintf(resultPtr, "%f", identifierEntry.value.floatData);
            default:
                break;
            }
            break;
        default:
            break;
        }
        current = current->next;
    }
    
    return result; // Return the concatenated string
}