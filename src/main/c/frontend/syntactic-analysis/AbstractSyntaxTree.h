#ifndef ABSTRACT_SYNTAX_TREE_HEADER
#define ABSTRACT_SYNTAX_TREE_HEADER

#include "../../shared/Logger.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeAbstractSyntaxTreeModule();

/** Shutdown module's internal state. */
void shutdownAbstractSyntaxTreeModule();

/**
 * This typedefs allows self-referencing types.
 */

typedef enum ExpressionType ExpressionType;
typedef enum FactorType FactorType;
typedef enum SentenceType SentenceType;

typedef enum BoolExpressionType BoolExpressionType;

typedef enum IntervalType IntervalType;

typedef enum StringPartType StringPartType;

typedef enum ArrayType ArrayType;

typedef struct Constant Constant;
typedef struct Expression Expression;
typedef struct Expressions Expressions;

typedef struct Factor Factor;
typedef struct Vector Vector;
typedef struct Program Program;

typedef struct Sentences Sentences;
typedef struct Sentence Sentence;
typedef struct IfSentence IfSentence;
typedef struct IfElseSentence IfElseSentence;
typedef struct ForSentence ForSentence;
typedef struct AssignSentence AssignSentence;
typedef struct Block Block;
typedef struct Interval Interval;
typedef struct BoolExpression BoolExpression;
typedef struct BoolFactor BoolFactor;
typedef struct ExpressionList ExpressionList;

typedef struct IntegerExpression IntegerExpression;
typedef struct IntegerFactor IntegerFactor;

typedef struct VectorExpression VectorExpression;
typedef struct VectorFactor VectorFactor;
typedef struct StringPartList StringPartList;
typedef struct StringPart StringPart;

typedef struct Array Array;

enum ExpressionType {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	FACTOR,
	MODULUS,
	GET_X,
	GET_Y,
	ARRAY_ACCESS
};


enum StringPartType {
	STRING_SEGMENT,
	IDENTIFIER_SEGMENT,
};


enum BoolExpressionType {
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	GREATER_THAN,
	LESS_THAN,
	EQUAL_TO,
	NOT_EQUAL,
	
	AND_TYPE,
	OR_TYPE,
	NOT_TYPE,
	BOOL_FACTOR
};

enum FactorType {
	IDENTIFIER_FACTOR,
	INTEGER_FACTOR,
	VECTOR_FACTOR,
	DECIMAL_FACTOR,
	PARENTHESIS_FACTOR
};

enum IntervalType {
	RANGE_INTERVAL,
	IDENTIFIER_INTERVAL,
};

enum SentenceType {
	IF_SENTENCE,
	IF_ELSE_SENTENCE,
	ASSIGN_SENTENCE,
	FOR_SENTENCE,
	FUNCTION_SENTENCE,
	ASSIGN_ARRAY_SENTENCE,
	ASSIGN_ARRAY_ELEMENT_SENTENCE,
	LOG_SENTENCE,
};

enum ArrayType {
	IDENTIFIER_ARRAY,
	INTERVAL_ARRAY,
	BASIC_ARRAY,
};



struct Program {
	Sentences * sentences;
};

struct Sentences {
	Sentences * sentences;
	Sentence * sentence;
};

struct Sentence {
	union {
		struct {
			char * assignIdentifier;
			Expression * assignExpression;
		};
		struct {
			char * assignArrayIdentifier;
			Array * assignArray;
		};
		struct {
			char * assignArrayElemIdentifier;
			Expression * assignArrayIndexExpression;
			Expression * assignArrayElementExpression;
		};
		struct {
			BoolExpression * ifBoolExpression;
			Block * ifBlock;
		};
		struct {
			BoolExpression * ifElseBoolExpression;
			Block * leftIfElseBlock;
			Block * rightIfElseBlock;
		};
		struct {
			char * forIdentifier;
			Array * forArray;
			Block * forBlock;
		};
		struct {
			char * functionIdentifier;
			ExpressionList * functionArguments;
		};
		struct {
			StringPartList * logString;
		};
	};
	SentenceType type;
};

struct StringPart {
	union {
		char * string;
		char * identifier;
	};
	StringPartType type;
};

struct StringPartList {
	StringPart * stringPart;
	StringPartList * next;
};

struct Block {
	Sentences * sentences;
};

struct Array {
	union {
		ExpressionList * expressionList;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		char * identifier;
	};
	ArrayType type;
};

struct BoolExpression {
	union {
		BoolFactor * boolFactor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		struct {
			BoolExpression * leftBoolExpression;
			BoolExpression * rightBoolExpression;
		};
		BoolExpression * boolExpression;
	};
	BoolExpressionType type;
};

struct BoolFactor {
	BoolExpression * boolExpression;
};

struct ExpressionList {
	Expressions * expressions;
};

struct Expressions {
	Expression * expression;
	Expressions * next;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
		struct {
			Array * array;
			Expression * indexExpression;
		};
		Expression * expression;
	};
	ExpressionType type;
};

struct Factor {
	union {
		char * identifier;
		Expression * expression;
		Vector * vector;
		int integerExpression;
		float floatExpression;
	};
	FactorType type;
};

struct Vector {
	Expression * x;
	Expression * y;
};




/**
 * Node recursive destructors.
 * TODO: seguir haciendo estos "Node Recursive Destructors"
 */
void releaseProgram(Program * program);
void releaseSentences(Sentences * sentences);
void releaseSentence(Sentence * sentence);
void releaseStringPart(StringPart * stringPart);
void releaseStringPartList(StringPartList * stringPartList);
void releaseBlock(Block * block);
void releaseBoolExpression(BoolExpression * boolExpression);
void releaseBoolFactor(BoolFactor * boolFactor);
void releaseExpressionList(ExpressionList * expressionList);
void releaseExpressions(Expressions * expressions);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseVector(Vector * vector);
void releaseArray(Array * array);




#endif
