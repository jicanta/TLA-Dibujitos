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
Constant * IntegerConstantSemanticAction(const int value);
Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Factor * VectorFactorSemanticAction(Vector * vector);
Factor * ConstantFactorSemanticAction(Constant * constant);
Factor * ExpressionFactorSemanticAction(Expression * expression);
Vector * VectorSemanticAction(Constant * left, Constant * right);


Program * SentencesProgramSemanticAction(CompilerState * compilerState, Sentences * sentences);
Sentences * EmptySentencesSemanticAction();
Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence);
Sentence * AssignSentenceSemanticAction(char * identifier, Expression * expression);
Sentence * IfSentenceSemanticAction(BoolExpression * boolExpression, Block * block);
Sentence * ForSentenceSemanticAction(char * identifier, Interval * interval, Block * block);
Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock);
Block * BlockSemanticAction(Sentences * sentences);
Interval * IntervalSemanticAction(Expression * leftExpression, Expression * rightExpression);
BoolExpression * BoolExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BoolExpressionType type);
BoolExpression * BoolBinaryExpressionSemanticAction(BoolExpression * leftBoolExpression, BoolExpression * rightBoolExpression, BoolExpressionType type);
BoolExpression * BoolUnaryExpressionSemanticAction(BoolExpression * boolExpression, BoolExpressionType type);
BoolExpression * BoolFactorExpressionSemanticAction(BoolFactor * boolFactor);
BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression);


#endif
