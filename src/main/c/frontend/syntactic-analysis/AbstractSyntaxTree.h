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

typedef enum FloatExpressionType FloatExpressionType;
typedef enum FloatFactorType FloatFactorType;
typedef enum SentenceType SentenceType;


typedef enum BoolExpressionType BoolExpressionType;
typedef enum ConstantType ConstantType;
typedef enum IntervalType IntervalType;
typedef enum StringPartType StringPartType;


typedef struct Constant Constant;
typedef struct FloatExpression FloatExpression;
typedef struct FloatFactor FloatFactor;
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
typedef struct StringPartList StringPartList;
typedef struct StringPart StringPart;

enum FloatExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
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

enum FloatFactorType {
	CONSTANT,
	EXPRESSION,
	VECTOR,
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
	LOG_SENTENCE,
};

enum ConstantType {
	INTEGER_CONSTANT,
	DECIMAL_CONSTANT,
	IDENTIFIER_CONSTANT,
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

struct Constant {
	union {
		int integer;
		float decimal;
		const char * identifier;
	};
	ConstantType type;
};

struct FloatFactor {
	union {
		Constant * constant;
		FloatExpression * floatExpression;
		Vector * vector;
	};
	FloatFactorType type;
};

struct Vector {
	Constant * left;
	Constant * right;
};

struct FloatExpression {
	union {
		FloatFactor * floatFactor;
		struct {
			FloatExpression * leftFloatExpression;
			FloatExpression * rightFloatExpression;
		};
	};
	FloatExpressionType type;
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
			FloatExpression * assignFloatExpression;
		};
		struct {
			char * assignArrayIdentifier;
			ExpressionList * arrayExpressionList;
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
			Interval * forInterval;
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

struct ExpressionList {
	FloatExpression * expression;
	ExpressionList * next;
};


struct IfSentence {
	BoolExpression * boolExpression;
	Block * block;
};

struct IfElseSentence {
	BoolExpression * boolExpression;
	Block * leftBlock;
	Block * rightBlock;
};

struct ForSentence {
	Interval * interval;
	Block * block;
};

struct AssignSentence {
	FloatExpression * floatExpression;
};

struct Block {
	Sentences * sentences;
};

struct Interval {
	union {
		struct {
			FloatExpression * leftFloatExpression;
			FloatExpression * rightFloatExpression;
		};
		struct {
			char * identifier;
		};
	};
	IntervalType type;
};

// TODO: Es muy probable que haya que separar esto en partes D:
struct BoolExpression {
	union {
		BoolFactor * boolFactor;
		struct {
			FloatExpression * leftFloatExpression;
			FloatExpression * rightFloatExpression;
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


/**
 * Node recursive destructors.
 * TODO: seguir haciendo estos "Node Recursive Destructors"
 */
void releaseConstant(Constant * constant);
void releaseFloatExpression(FloatExpression * floatExpression);
void releaseFloatFactor(FloatFactor * floatFactor);
void releaseProgram(Program * program);
void releaseSentences(Sentences * sentences);
void releaseSentence(Sentence * sentence);
void releaseBlock(Block * block);
void releaseInterval(Interval * interval);
void releaseBoolExpression(BoolExpression * boolExpression);
void releaseBoolFactor(BoolFactor * boolFactor);
void releaseVector(Vector * vector);
void releaseExpressionList(ExpressionList * expressionList);
void releaseStringPartList(StringPartList * stringPartList);
void releaseStringPart(StringPart * stringPart);


#endif
