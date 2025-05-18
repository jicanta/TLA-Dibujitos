#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Constant * DecimalConstantSemanticAction(const float decimal);
Constant * IdentifierConstantSemanticAction(const char * identifier);
Constant * IntegerConstantSemanticAction(const int value);
FloatExpression * FloatArithmeticExpressionSemanticAction(FloatExpression * leftFloatExpression, FloatExpression * rightFloatExpression, FloatExpressionType type);
FloatExpression * FloatFactorExpressionSemanticAction(FloatFactor * factor);
FloatExpression * VectorDotExpressionSemanticAction(VectorExpression * vectorExpression, FloatExpressionType type);
FloatFactor * FloatConstantFactorSemanticAction(Constant * constant);
FloatFactor * FloatExpressionFactorSemanticAction(FloatExpression * expression);
Vector * VectorSemanticAction(Constant * left, Constant * right);


Program * SentencesProgramSemanticAction(CompilerState * compilerState, Sentences * sentences);
Sentences * EmptySentencesSemanticAction();
Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence);
Sentence * AssignFloatSentenceSemanticAction(char * identifier, FloatExpression * expression);
Sentence * AssignIntegerSentenceSemanticAction(char * identifier, IntegerExpression * expression);
Sentence * AssignVectorSentenceSemanticAction(char * identifier, VectorExpression * vectorExpression);

Sentence * IfSentenceSemanticAction(BoolExpression * boolExpression, Block * block);
Sentence * ForSentenceSemanticAction(char * identifier, Interval * interval, Block * block);
Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock);
Sentence * AssignArraySentenceSemanticAction(char * identifier, ExpressionList * expressionList);
Block * BlockSemanticAction(Sentences * sentences);
Interval * IntervalSemanticAction(IntegerExpression * leftIntegerExpression, IntegerExpression * rightIntegerExpression);
Interval * IntervalIdentifierSemanticAction(char * identifier);
BoolExpression * BoolExpressionSemanticAction(FloatExpression * leftFloatExpression, FloatExpression * rightFloatExpression, BoolExpressionType type);
BoolExpression * BoolBinaryExpressionSemanticAction(BoolExpression * leftBoolExpression, BoolExpression * rightBoolExpression, BoolExpressionType type);
BoolExpression * BoolUnaryExpressionSemanticAction(BoolExpression * boolExpression, BoolExpressionType type);
BoolExpression * BoolFactorExpressionSemanticAction(BoolFactor * boolFactor);
BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression);
Sentence * FunctionSentenceSemanticAction(char * identifier, ExpressionList * functionArguments);
ExpressionList * ExpressionListSemanticAction(ExpressionList * expressionList, GenericExpression * expression);
ExpressionList * EmptyExpressionListSemanticAction();
Sentence * LogSentenceSemanticAction(StringPartList * stringPartList);
StringPartList * appendStringPartList(StringPartList * stringPartList, StringPart *stringPart);
StringPartList * createStringPartList(StringPart * stringPart);
StringPart * createStringSegment(char * string);
StringPart * createStringInterpolation(char * identifier);
IntegerExpression * IntegerArithmeticExpressionSemanticAction(IntegerExpression * leftIntegerExpression, IntegerExpression * rightIntegerExpression, IntegerExpressionType type);
FloatFactor * IntegerToFloatFactorSemanticAction(IntegerExpression * integerExpression);

IntegerFactor * IntegerExpressionFactorSemanticAction(IntegerExpression * integerExpression);
IntegerFactor * IntegerConstantFactorSemanticAction(int integer);
IntegerFactor * IntegerIdentifierFactorSemanticAction(char * identifier);

IntegerExpression * IntegerFactorExpressionSemanticAction(IntegerFactor * integerFactor);

VectorExpression * VectorArithmeticExpressionSemanticAction(VectorExpression * leftVectorExpression, VectorExpression * rightVectorExpression, VectorExpressionType type);
VectorExpression * VectorFloatArithmeticExpressionSemanticAction(VectorExpression * leftVectorExpression, FloatExpression * rightFloatExpression, VectorExpressionType type);
VectorExpression * VectorFactorExpressionSemanticAction(VectorFactor * vectorFactor);
VectorFactor * VectorExpressionFactorSemanticAction(VectorExpression * vectorExpression);
VectorFactor * VectorIdentifierFactorSemanticAction( char * identifier);
VectorFactor * VectorFactorSemanticAction(Vector * vector);

GenericExpression * FloatGenericExpressionSemanticAction(FloatExpression * floatExpression);
GenericExpression * IntegerGenericExpressionSemanticAction(IntegerExpression * integerExpression); 
GenericExpression * VectorGenericExpressionSemanticAction(VectorExpression * vectorExpression);

#endif
