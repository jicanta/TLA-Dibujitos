#include "SymbolTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Scopes.h"

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

void printSymbolValue(SymbolEntry entry) {
    switch (entry.type) {
        case INTEGER_TYPE:
            printf("%d", entry.value.integerData);
            break;
        case FLOAT_TYPE:
            printf("%f", entry.value.floatData);
            break;
        case VECTOR_TYPE:
            printf("(%f, %f)", entry.value.vectorData.x, entry.value.vectorData.y);
            break;
        case FUNCTION_TYPE:
            printf("{ret: %s, params: ", symbolTypeToString(entry.value.functionData.returnType));
            if(entry.value.functionData.parameterCount == -1) {
                printf("infinite %s }", symbolTypeToString(entry.value.functionData.parameterType[0]));
                break;
            }
            for(int i = 0; i < entry.value.functionData.parameterCount; i++) {
                printf("%s, ", symbolTypeToString(entry.value.functionData.parameterType[i]));
            }
            printf(" }");
            break;
        case ARRAY_TYPE:
            switch (entry.value.arrayData.dataType) {
            case IDENTIFIER_ARRAY:
                /* code */
                break;
            case INTERVAL_ARRAY:
                printf(" (interval) {left: %d, right: %d}",
                    entry.value.arrayData.elements[0].value.integerData,
                    entry.value.arrayData.elements[1].value.integerData);
                break;
            case BASIC_ARRAY:
                BasicType *arrayElements = entry.value.arrayData.elements;
                printf("%s[%d]{", symbolTypeToString(entry.value.arrayData.elements[0].type), entry.value.arrayData.size);
                for(int i = 0; i < entry.value.arrayData.size; i++) {
                    if (i > 0) {
                        printf(", ");
                    }
                    switch (arrayElements[i].type) {
                        case INTEGER_TYPE:
                            printf("%d", arrayElements[i].value.integerData);
                            break;
                        case FLOAT_TYPE:
                            printf("%f", arrayElements[i].value.floatData);
                            break;
                        case VECTOR_TYPE:
                            printf("(%f, %f)", arrayElements[i].value.vectorData.x, arrayElements[i].value.vectorData.y);
                            break;
                        default:
                            printf("(unknown type)");
                            break;
                    }
                }
                break;
            default:
                break;
            }
            printf("}");
            break;
        default:
            printf("[(unknown) %s]", entry.identifier);
            break;
    }
}
// Function to print an element
void printSymbolEntry(const SymbolEntry entry) {
    printf("[%s[%d]%s = ", entry.identifier, entry.scope, symbolTypeToString(entry.type));
    printSymbolValue(entry);
    puts("]");
}


// Function to print the LinkedList
void printSymbolTable(SymbolTable* symbolTable) {
    const SymbolEntryNode* head = symbolTable->head; // symbolTable is just another way of referring to the head of the linked list

    const SymbolEntryNode* temp = head;
    while (temp != NULL) {
        printSymbolEntry(temp->entry);
        temp = temp->next;
    }
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

void updateSymbol(SymbolTable* symbolTable, const SymbolEntry data) {
    SymbolEntryNode* head = symbolTable->head;
    
    while (head != NULL) {
        if (strcmp(head->entry.identifier, data.identifier) == 0) {
            head->entry.value = data.value;
            return;
        }
        head = head->next;
    }
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

SymbolEntry getSymbolEntryWithScope(const SymbolTable* symbolTable, const char* identifier, ScopesStack* stack) {
    SymbolEntryNode* head = symbolTable->head;

    while (head != NULL) {
        if (strcmp(head->entry.identifier, identifier) == 0) {
            SymbolEntry ret = head->entry; // Return the found entry
            if (containsScopesStack(stack, ret.scope)) {
                return ret; // Return the found entry if it matches the current scope
            }
        }
        head = head->next;
    }

    SymbolEntry emptyEntry = {0}; // Initialize an empty SymbolEntry
    emptyEntry.identifier = NULL;
    emptyEntry.type = NULL_TYPE; // Set to an invalid type
    // Initialize other fields of emptyEntry as needed
    return emptyEntry;
}

void setDefaultColors(SymbolTable* symbolTable, int scopeInit) {
    SymbolEntry redColor = {
        .identifier = "RED",
        .type = INTEGER_TYPE,
        .value.integerData = 0xFF0000, // Hexadecimal representation of red color
        .scope = scopeInit
    };
    insertSymbol(symbolTable, redColor);

    SymbolEntry greenColor = {
        .identifier = "GREEN",
        .type = INTEGER_TYPE,
        .value.integerData = 0x00FF00, // Hexadecimal representation of green color
        .scope = scopeInit
    };
    insertSymbol(symbolTable, greenColor);

    SymbolEntry blueColor = {
        .identifier = "BLUE",
        .type = INTEGER_TYPE,
        .value.integerData = 0x0000FF, // Hexadecimal representation of blue color
        .scope = scopeInit
    };
    insertSymbol(symbolTable, blueColor);

    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "YELLOW",
        .type = INTEGER_TYPE,
        .value.integerData = 0xFFFF00, // Hexadecimal representation of yellow color
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "BLACK",
        .type = INTEGER_TYPE,
        .value.integerData = 0x000000, // Hexadecimal representation of black color
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "ORANGE",
        .type = INTEGER_TYPE,
        .value.integerData = 0xFFA500, // Hexadecimal representation of orange color
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "INDIGO",
        .type = INTEGER_TYPE,
        .value.integerData = 0x4B0082, // Hexadecimal representation of indigo color
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "PURPLE",
        .type = INTEGER_TYPE,
        .value.integerData = 0x800080, // Hexadecimal representation of purple color
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "BACK",
        .type = INTEGER_TYPE,
        .value.integerData = 0,
        .scope = scopeInit
    });

    insertSymbol(symbolTable, (SymbolEntry){
        .identifier = "FRONT",
        .type = INTEGER_TYPE,
        .value.integerData = 1,
        .scope = scopeInit
    });
}
// Function to set all default functions
void setDefaultFunctions(SymbolTable* symbolTable, int scopeInit) {
    setDefaultColors(symbolTable, scopeInit);
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
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, circleFunction);

    SymbolType* lineDataTypes = malloc(2 * sizeof(SymbolType));
    lineDataTypes[0] = VECTOR_TYPE;
    lineDataTypes[1] = VECTOR_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "line",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = lineDataTypes,
            .parameterCount = 2,
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* curveDataTypes = malloc(1 * sizeof(SymbolType));
    curveDataTypes[0] = VECTOR_TYPE;
    const SymbolEntry curveFunction = {
        .identifier = "curve",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = curveDataTypes,
            .parameterCount = -1, // Indicates that the function can take an infinite number of parameters;
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, curveFunction);

    SymbolType* fillDataTypes = malloc(1 * sizeof(SymbolType));
    fillDataTypes[0] = INTEGER_TYPE;
    const SymbolEntry fillFunction = {
        .identifier = "fill",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = fillDataTypes,
            .parameterCount = 1,
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, fillFunction);

    SymbolType* strokeDataTypes = malloc(1 * sizeof(SymbolType));
    strokeDataTypes[0] = INTEGER_TYPE;  
    const SymbolEntry strokeFunction = {
        .identifier = "stroke",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = strokeDataTypes,
            .parameterCount = 1,
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, strokeFunction);

    SymbolType* zDataTypes = malloc(1 * sizeof(SymbolType));
    zDataTypes[0] = INTEGER_TYPE;  
    const SymbolEntry zFunction = {
        .identifier = "layer",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = zDataTypes,
            .parameterCount = 1,
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, zFunction);

    SymbolType* sqrtDataTypes = malloc(1 * sizeof(SymbolType));
    sqrtDataTypes[0] = FLOAT_TYPE;  
    const SymbolEntry sqrtFunction = {
        .identifier = "sqrt",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = sqrtDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    };
    insertSymbol(symbolTable, sqrtFunction);

    SymbolType* cosDataTypes = malloc(1 * sizeof(SymbolType));
    cosDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "cos",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = cosDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* sinDataTypes = malloc(1 * sizeof(SymbolType));
    sinDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "sin",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = sinDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* tanDataTypes = malloc(1 * sizeof(SymbolType));
    tanDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "tan",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = tanDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* absDataTypes = malloc(1 * sizeof(SymbolType));
    absDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "abs",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = absDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* logfDataTypes = malloc(1 * sizeof(SymbolType));
    logfDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "logf",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = logfDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* powfDataTypes = malloc(2 * sizeof(SymbolType));
    powfDataTypes[0] = FLOAT_TYPE;
    powfDataTypes[1] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "powf",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = powfDataTypes,
            .parameterCount = 2,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* roundfDataTypes = malloc(1 * sizeof(SymbolType));
    roundfDataTypes[0] = FLOAT_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "roundf",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = roundfDataTypes,
            .parameterCount = 1,
            .returnType = FLOAT_TYPE,
        },
        .scope = scopeInit
    });

    SymbolType* polygonDataTypes = malloc(1 * sizeof(SymbolType));
    polygonDataTypes[0] = VECTOR_TYPE;
    insertSymbol(symbolTable, (SymbolEntry) {
        .identifier = "polygon",
        .type = FUNCTION_TYPE,
        .value.functionData = {
            .parameterType = polygonDataTypes,
            .parameterCount = -1, // Indicates that the function can take an infinite number of vector parameters
            .returnType = NULL_TYPE,
        },
        .scope = scopeInit
    });
    
}

SymbolType typeOfFactor(Factor *factor) {
    SymbolType leftType, rightType = INVALID_TYPE;
    if(factor == NULL) return INVALID_TYPE;
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
                return ARRAY_TYPE;
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
            return FLOAT_TYPE; // Assumes that vector has two floats

        case FACTOR:
            return typeOfFactor(expression->factor);
        case ARRAY_ACCESS:
            resultType2 = typeOfExpression(expression->indexExpression);
            SymbolEntry arrayEntry = getSymbolEntry(currentCompilerState()->symbolTable, expression->array->identifier); // Get the symbol entry for the array            
            if (resultType2 != INTEGER_TYPE) {
                return INVALID_TYPE; // Index must be an integer
            }
        
            return arrayEntry.value.arrayData.elements[0].type; // Return the type of the array

        case FUNCTION_EXPRESSION:
            // Types of function arguments are not checked here, they are checked in the semantic analysis phase

            SymbolEntry functionEntry = getSymbolEntry(currentCompilerState()->symbolTable, expression->functionIdentifier);
            return functionEntry.value.functionData.returnType; // Return the type of the function
   
    }
    return INVALID_TYPE;
}


int boolExpressionIsValid(BoolExpression* expression) {
    if (!expression) return 0; // NULL expression is invalid
    
    switch (expression->type) {
        // Boolean operators
        case AND_TYPE:
        case OR_TYPE:
            return boolExpressionIsValid(expression->leftBoolExpression) && 
                   boolExpressionIsValid(expression->rightBoolExpression);
        
        case NOT_TYPE:
            return boolExpressionIsValid(expression->boolExpression);
        
        // Comparison operators
        case GREATER_OR_EQUAL:
        case LESS_OR_EQUAL:
        case GREATER_THAN:
        case LESS_THAN:
        case EQUAL_TO:
        case NOT_EQUAL:
            {
                SymbolType leftType = typeOfExpression(expression->leftExpression);
                SymbolType rightType = typeOfExpression(expression->rightExpression);
                
                // Check if both expressions are valid and have the same type
                if (leftType == INVALID_TYPE || rightType == INVALID_TYPE) {
                    return 0; // Invalid if either expression has an invalid type
                }
                
                return leftType == rightType; // Valid only if both have the same type
            }
        
        case BOOL_FACTOR:
            // Handle the case where boolFactor contains a nested boolean expression
            if (expression->boolFactor && expression->boolFactor->boolExpression) {
                return boolExpressionIsValid(expression->boolFactor->boolExpression);
            }
            return 0; // Invalid if boolFactor is NULL or doesn't contain a boolean expression
            
        default:
            return 0; // Unknown boolean expression type is invalid
    }
}