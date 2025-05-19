#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction();
void EndMultilineCommentLexemeAction();
void IgnoredLexemeAction(char * text);

Token ArithmeticOperatorLexemeAction(Token token, char *text);
Token IntegerLexemeAction(char * text);
Token DecimalLexemeAction(char * text);
Token ParenthesisLexemeAction(Token token, char *text);

Token UnknownLexemeAction(char *text);

// NUESTRO

Token BinaryOperatorLexemeAction(Token token, char *text);
Token UnaryOperatorLexemeAction(Token token, char *text);
Token AssignmentLexemeAction();
Token CommaLexemeAction();
Token SemicolonLexemeAction();
Token DotLexemeAction();
Token ColonLexemeAction();
Token ImportLexemeAction();
Token IfLexemeAction();
Token ElseLexemeAction();
Token InLexemeAction();
Token ForLexemeAction();
Token BracesLexemeAction(Token token, char *text);
Token BracketsLexemeAction(Token token, char *text);
Token LogLexemeAction();
Token BeginInterpolatedStringLexemeAction();
Token EndInterpolatedStringLexemeAction();
Token IdentifierLexemeAction(char * text_pointer, int text_length);
Token StringLexemeAction(char * text_pointer, int text_length);
Token FloatKeywordLexemeAction();
Token IntKeywordLexemeAction();
Token VectorKeywordLexemeAction();

Token XParamLexemeAction();
Token YParamLexemeAction();

#endif
