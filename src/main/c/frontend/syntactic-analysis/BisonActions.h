#include "../../shared/CompilerState.h"
#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

struct CompilerState; // forward declaration
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
Sentence * ForSentenceSemanticAction(char * identifier, Array * array, Block * block);
Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock);
Sentence * AssignArraySentenceSemanticAction(char * identifier, Array * array);
Block * BlockSemanticAction(Sentences * sentences);
BoolExpression * BoolBinaryExpressionSemanticAction(BoolExpression * leftBoolExpression, BoolExpression * rightBoolExpression, BoolExpressionType type);
BoolExpression * BoolUnaryExpressionSemanticAction(BoolExpression * boolExpression, BoolExpressionType type);
BoolExpression * BoolFactorExpressionSemanticAction(BoolFactor * boolFactor);
BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression);
Sentence * FunctionSentenceSemanticAction(char * identifier, ExpressionList * functionArguments);
Sentence * LogSentenceSemanticAction(StringPartList * stringPartList);
StringPartList * appendStringPartList(StringPartList * stringPartList, StringPart *stringPart);
StringPartList * createStringPartList(StringPart * stringPart);
StringPart * createStringSegment(char * string);
StringPart * createStringInterpolation(char * identifier);
Factor * VectorFactorSemanticAction(Vector * vector);

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type);
Expression * FactorExpressionSemanticAction(Factor * factor);
Expression * DotExpressionSemanticAction(Expression * expression, ExpressionType type);
Expression * ArrayAccessExpressionSemanticAction(Array * array, Expression * indexExpression);
Expression * FunctionExpressionSemanticAction(char * identifier, ExpressionList * functionArguments);
Factor * IdentifierFactorSemanticAction(char * identifier);
Factor * IntegerFactorSemanticAction(int integer);
Factor * DecimalFactorSemanticAction(float decimal);
Factor * ParenthesisFactorSemanticAction(Expression * expression);
ExpressionList * FilledExpressionListSemanticAction(Expressions * expressions);
ExpressionList * EmptyExpressionListSemanticAction();
Expressions * ExpressionsSemanticAction(Expressions * expressions, Expression * expression);
BoolExpression * BoolComparisonExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BoolExpressionType type);
Array * IntervalArraySemanticAction(Expression * leftExpression, Expression * rightExpression);
Array * IdentifierArraySemanticAction(char * identifier);
Array * BasicArraySemanticAction(ExpressionList * expressionList);
Sentence * AssignArrayElementSentenceSemanticAction(char * identifier, Expression * indexExpression, Expression * expression);
Sentence * ImportSentenceSemanticAction(StringPartList * importPath);
Sentence * AssignVectorComponentSemanticAction(char *identifier, Expression *expression, ExpressionType type);
int InsertForLoopIterator(char * identifier, Array * array);
void StartScope();

#endif
