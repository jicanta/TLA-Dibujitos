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
typedef enum IntegerExpressionType IntegerExpressionType;
typedef enum IntegerFactorType IntegerFactorType;

typedef enum VectorExpressionType VectorExpressionType;
typedef enum VectorFactorType VectorFactorType;
typedef enum StringPartType StringPartType;

typedef enum GenericExpressionType GenericExpressionType;


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

typedef struct IntegerExpression IntegerExpression;
typedef struct IntegerFactor IntegerFactor;

typedef struct VectorExpression VectorExpression;
typedef struct VectorFactor VectorFactor;
typedef struct StringPartList StringPartList;
typedef struct StringPart StringPart;

typedef struct GenericExpression GenericExpression;

enum FloatExpressionType {
	ADDITION,
	SUBTRACTION,
	MULTIPLICATION,
	DIVISION,
	FACTOR,
	GET_X,
	GET_Y
};

enum IntegerExpressionType {
	INT_ADDITION,
	INT_SUBTRACTION,
	INT_MULTIPLICATION,
	INT_DIVISION,
	INT_MODULUS,
	INT_FACTOR
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
	INTEGER_TO_FLOAT
};

enum IntervalType {
	RANGE_INTERVAL,
	IDENTIFIER_INTERVAL,
};

enum SentenceType {
	IF_SENTENCE,
	IF_ELSE_SENTENCE,
	ASSIGN_SENTENCE,
	ASSIGN_INT_SENTENCE,
	ASSIGN_VECTOR_SENTENCE,
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

enum IntegerFactorType {
	INT_CONSTANT,
	INT_EXPRESSION,
	INT_IDENTIFIER
};

enum VectorExpressionType {
	VEC_ADDITION,
	VEC_SUBTRACTION,
	VEC_MULTIPLICATION,
	VEC_DIVISION,
	VEC_FACTOR
};
enum VectorFactorType {
	VEC_VECTOR,
	VEC_EXPRESSION,
	VEC_IDENTIFIER
};

enum GenericExpressionType {
	GENERIC_FLOAT,
	GENERIC_INTEGER,
	GENERIC_VECTOR
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
		IntegerExpression * integerExpression;
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
		VectorExpression * vectorExpression;
	};
	FloatExpressionType type;
};

struct VectorExpression {
	union {
		VectorFactor * vectorFactor;
		struct {
			VectorExpression * leftVectorExpression;
			VectorExpression * rightVectorExpression;
		};
		struct {
			VectorExpression * vectorExpression;
			FloatExpression * floatExpression;
		};
	};
	VectorExpressionType type;
};

struct VectorFactor {
	union {
		Vector * vector;
		VectorExpression * vectorExpression;
		char * identifier;
	};
	VectorFactorType type;
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
			char * assignFloatIdentifier;
			FloatExpression * assignFloatExpression;
		};
		struct {
			char * assignIntegerIdentifier;
			IntegerExpression * assignIntegerExpression;
		};
		struct {
			char * assignVectorIdentifier;
			VectorExpression * assignVectorExpression;
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
	GenericExpression * expression;
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
			IntegerExpression * leftIntegerExpression;
			IntegerExpression * rightIntegerExpression;
		};
		struct {
			char * identifier;
		};
	};
	IntervalType type;
};
// TODO: quizás conviene sacar el IntervalType

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

struct IntegerExpression {
	union {
		IntegerFactor * integerFactor;
		struct {
			IntegerExpression * leftIntegerExpression;
			IntegerExpression * rightIntegerExpression;
		};
	};
	IntegerExpressionType type;
};

struct IntegerFactor {
	union {
		int integer;
		IntegerExpression * integerExpression;
		char * identifier;
	};
	IntegerFactorType type;
};

struct GenericExpression {
	union {
		FloatExpression * floatExpression;
		IntegerExpression * integerExpression;
		VectorExpression * vectorExpression;
	};
	GenericExpressionType genericExpressionType;
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
void releaseIntegerExpression(IntegerExpression * integerExpression);
void releaseIntegerFactor(IntegerFactor * integerFactor);
void releaseVectorExpression(VectorExpression * vectorExpression);
void releaseVectorFactor(VectorFactor * vectorFactor);

void releaseStringPartList(StringPartList * stringPartList);
void releaseStringPart(StringPart * stringPart);

void releaseGenericExpression(GenericExpression * genericExpression);


#endif
