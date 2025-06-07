/* "TODO"
el objetivo de esta etapa seria:
    - agregarle tipos de dato a las asignaciones 
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

*/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef enum {
    // Types for expressions
    INTEGER_TYPE,
    FLOAT_TYPE,
    VECTOR_TYPE,
    // Other types
    ARRAY_TYPE,
    FUNCTION_TYPE,
    // BOOLEAN_TYPE, TODO decidir si sacarlo
    // STRING_TYPE, 
} SymbolType;



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