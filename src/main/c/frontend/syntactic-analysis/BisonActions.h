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

Vector * VectorSemanticAction(Expression * x, Expression * y);
Program * SentencesProgramSemanticAction(CompilerState * compilerState, Sentences * sentences);
Sentences * EmptySentencesSemanticAction();
Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence);
Sentence * AssignSentenceSemanticAction(char * identifier, Expression * expression);
Sentence * IfSentenceSemanticAction(BoolExpression * boolExpression, Block * block);
Sentence * ForSentenceSemanticAction(char * identifier, Interval * interval, Block * block);
Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock);
Sentence * AssignArraySentenceSemanticAction(char * identifier, ExpressionList * expressionList);
Block * BlockSemanticAction(Sentences * sentences);
Interval * IntervalIdentifierSemanticAction(char * identifier);
BoolExpression * BoolBinaryExpressionSemanticAction(BoolExpression * leftBoolExpression, BoolExpression * rightBoolExpression, BoolExpressionType type);
BoolExpression * BoolUnaryExpressionSemanticAction(BoolExpression * boolExpression, BoolExpressionType type);
BoolExpression * BoolFactorExpressionSemanticAction(BoolFactor * boolFactor);
BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression);
Sentence * FunctionSentenceSemanticAction(char * identifier, ExpressionList * functionArguments);
ExpressionList * EmptyExpressionListSemanticAction();
Sentence * LogSentenceSemanticAction(StringPartList * stringPartList);
StringPartList * appendStringPartList(StringPartList * stringPartList, StringPart *stringPart);
StringPartList * createStringPartList(StringPart * stringPart);
StringPart * createStringSegment(char * string);
StringPart * createStringInterpolation(char * identifier);
Factor * VectorFactorSemanticAction(Vector * vector);

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Expression * DotExpressionSemanticAction(Expression * expression, ExpressionType type);
Factor * IdentifierFactorSemanticAction(char * identifier);
Factor * IntegerFactorSemanticAction(int integer);
Factor * DecimalFactorSemanticAction(float decimal);
Factor * ParenthesisFactorSemanticAction(Expression * expression);
ExpressionList * ExpressionListSemanticAction(Expressions * expressions);
Expressions * ExpressionsSemanticAction(Expressions * expressions, Expression * expression);
BoolExpression * BoolComparisonExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BoolExpressionType type);
Interval * IntervalSemanticAction(Expression * leftExpression, Expression * rightExpression);


#endif
