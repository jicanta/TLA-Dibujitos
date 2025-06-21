#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "Scopes.h"
#include "./../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "./../../frontend/syntactic-analysis/SyntacticAnalyzer.h"

typedef enum {
    // Types for expressions
    NULL_TYPE,
    INTEGER_TYPE,
    FLOAT_TYPE,
    VECTOR_TYPE,
    // Other types
    ARRAY_TYPE,
    FUNCTION_TYPE,
    BOOLEAN_TYPE, 
    STRING_TYPE,
    INVALID_TYPE,
} SymbolType;


typedef struct VectorData {float x; float y;} VectorData;

typedef struct BasicType {
    SymbolType type; // Type of the basic type
    union {
        int integerData; // Integer value for integer identifiers
        float floatData; // Float value for float identifiers
        VectorData vectorData; // Vector details
    } value; // Value associated with the basic type
} BasicType;


typedef struct SymbolEntry SymbolEntry;
typedef struct SymbolEntry {
    char* identifier; // Identifier name
    SymbolType type; // Type of the identifier
    union {
        int integerData; // Integer value for integer identifiers
        float floatData; // Float value for float identifiers
        VectorData vectorData; // Vector details
        struct {
            SymbolType* parameterType; // Data types for the function parameters. Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
            int parameterCount; // Number of parameters
            SymbolType returnType; // Type of the identifier
            void (*functionPointer)(); // Pointer to the function implementation
        } functionData; // Function details
        struct {
            BasicType* elements; // Elements of the array of the same type. Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
            int size; // Size of the array
            SymbolType dataType; // Data type for the array. Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
        } arrayData; // Array details
    } value; // Value associated with the identifier
    int scope;
} SymbolEntry;

// Define the Node structure
typedef struct SymbolEntryNode SymbolEntryNode;
typedef struct SymbolEntryNode {
    SymbolEntry entry;
    SymbolEntryNode* next;
} SymbolEntryNode;


typedef struct SymbolTable {
    SymbolEntryNode* head; // Pointer to the head of the linked list
} SymbolTable;

SymbolTable *createSymbolTable();
SymbolEntry getSymbolEntry(SymbolTable* symbolTable, const char* identifier);
void insertSymbol(SymbolTable* symbolTable, SymbolEntry  data);
void printSymbolTable( SymbolTable* symbolTable);
void printSymbolEntry(const SymbolEntry entry);
void freeSymbolTable( SymbolTable* symbolTable);
void setDefaultFunctions(SymbolTable* symbolTable, int scopeInit);
SymbolType typeOfExpression(Expression* expression);
int intValueExpression(Expression* expression);
float floatValueExpression(Expression* expression);
const char *symbolTypeToString(SymbolType type);
VectorData vectorValueExpression(Expression *expression);
void updateSymbol(SymbolTable* symbolTable, const SymbolEntry data);
SymbolEntry getSymbolEntryWithScope(const SymbolTable* symbolTable, const char* identifier, ScopesStack* stack);
int boolExpressionIsValid(BoolExpression* expression);
void printSymbolValue(SymbolEntry entry);
/* LISTA DE CONSTANTES PREDEFINIDAS
z-layer-control:
    FRONT
    BACK

colores:
    INVISIBLE
    RED
    GREEN
    BLUE
    YELLOW
    BLACK
    WHITE
    GRAY
    ORANGE


funciones que dibujan pero no retornan valor:
    circle
    stroke
    fill
    curve

funciones que retornan valor pero no dibujan:
    sqrt
    dist
*/


#endif