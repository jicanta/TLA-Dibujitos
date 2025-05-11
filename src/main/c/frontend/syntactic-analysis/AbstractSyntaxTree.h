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
typedef enum ConstantType ConstantType;


typedef struct Constant Constant;
typedef struct Expression Expression;
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

/**
 * Node types for the Abstract Syntax Tree (AST).
 */

enum ExpressionType {
	ADDITION,
	DIVISION,
	FACTOR,
	MULTIPLICATION,
	SUBTRACTION
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
	CONSTANT,
	EXPRESSION,
	VECTOR,
};

enum SentenceType {
	IF_SENTENCE,
	IF_ELSE_SENTENCE,
	ASSIGN_SENTENCE,
	FOR_SENTENCE,
};

enum ConstantType {
	INTEGER,
	DECIMAL,
};

struct Constant {
	union {
		int integer;
		float decimal;
	};
	ConstantType type;
};

struct Factor {
	union {
		Constant * constant;
		Expression * expression;
		Vector * vector;
	};
	FactorType type;
};

struct Vector {
	Constant * left;
	Constant * right;
};

struct Expression {
	union {
		Factor * factor;
		struct {
			Expression * leftExpression;
			Expression * rightExpression;
		};
	};
	ExpressionType type;
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
	};
	SentenceType type;
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
	Expression * expression;
};

struct Block {
	Sentences * sentences;
};

struct Interval {
	Expression * leftExpression;
	Expression * rightExpression;
};

// TODO: Es muy probable que haya que separar esto en partes D:
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


/**
 * Node recursive destructors.
 * TODO: seguir haciendo estos "Node Recursive Destructors"
 */
void releaseConstant(Constant * constant);
void releaseExpression(Expression * expression);
void releaseFactor(Factor * factor);
void releaseProgram(Program * program);
void releaseSentences(Sentences * sentences);
void releaseSentence(Sentence * sentence);
void releaseBlock(Block * block);
void releaseInterval(Interval * interval);
void releaseBoolExpression(BoolExpression * boolExpression);
void releaseBoolFactor(BoolFactor * boolFactor);



#endif
