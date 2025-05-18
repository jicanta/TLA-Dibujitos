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

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DecimalLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);

// NUESTRO

Token BinaryOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token UnaryOperatorLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token AssignmentLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token CommaLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token SemicolonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token DotLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ColonLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ImportLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token IfLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ElseLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token InLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token ForLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);
Token BracesLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token BracketsLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, Token token);
Token LogLexemeAction(LexicalAnalyzerContext *);
void BeginInterpolatedStringLexemeAction(LexicalAnalyzerContext *);
void EndInterpolatedStringLexemeAction(LexicalAnalyzerContext *);
Token IdentifierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, char * text_pointer, int text_length);
Token StringLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, char * text_pointer, int text_length);
Token FloatKeywordLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext);


#endif
