/* "TODO"
el objetivo de esta etapa seria:
    - agregarle tipos de dato a las asignaciones (al final esto no hace falta xd)
        ejemplo: float num = 3.3;
        ejemplo: vector vec = (1,3);
        ejemplo: int entero = 4;
    - meter las variables declaradas
    - resolver tipos de datos de las expresiones
    - detectar el uso de variables inexistentes
    - detectar operaciones incompatibles sobre tipos de datos
        ejemplo: funcion + 3;
        ejemplo: numero();
    - ver si hacer scopes

Despues
    - crear las funciones default y meterlas en la tabla
        - Crear archivo con todas las funciones

TABLA DE SIMBOLOS:
ID      TYPE        INITIALIZATION

foo     FUNCTION    REF-funcion
numero  INTEGER     3



struct de funcion
lista de tipos: int (retorno), float (param1), float (param2)


Notas Alex:
- Verificar tipos en funciones CON RETORNO
- Manejar Scopes (FOR, IF ELSE)
- - Leer el documento de scopes
- Otros casos derivados:
- - ¿Qué errores pueden aparecer al manejar arrays en FORs?
- - Error de buscar en un array un valor demasiado alto
- - Error de buscar en un array un valor negativo

*/
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "./../../frontend/syntactic-analysis/abstractSyntaxTree.h"
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
            SymbolType* dataTypes; // Data types for the function parameters (Null Terminated). Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
            int hasInfiniteParameters; // Indicates if the function can take an infinite number of parameters (from the last data type)
            SymbolType returnType; // Type of the identifier
            void (*functionPointer)(); // Pointer to the function implementation
        } functionData; // Function details
        struct {
            BasicType* elements; // Elements of the array of the same type (Null Terminated). Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
            SymbolType dataType; // Data type for the array. Must be only INTEGER_TYPE, FLOAT_TYPE, VECTOR_TYPE
        } arrayData; // Array details
    } value; // Value associated with the identifier

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
void freeSymbolTable( SymbolTable* symbolTable);
void setDefaultFunctions(SymbolTable* symbolTable);
SymbolType typeOfExpression(Expression* expression);
// BasicType calculate(Expression* expression);


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