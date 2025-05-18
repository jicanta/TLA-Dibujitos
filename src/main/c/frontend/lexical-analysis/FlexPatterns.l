%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT
%x INTERP_STRING

%x INT_ASSIGNMENT
%x FLOAT_ASSIGNMENT
%x VECTOR_ASSIGNMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */
string            \"([^\\\"{}]|\\.)*\"
digit              [0-9]
decimal            [+-]?(([0-9]+\.[0-9]*)|(\.[0-9]+))
char               [a-zA-Z_]
identifier         {char}({char}|{digit})*
comment           \/\/[^\n]*
%%

\"                              { BEGIN(INTERP_STRING); BeginInterpolatedStringLexemeAction(createLexicalAnalyzerContext()); }

<INTERP_STRING>[^\\"{}]+        { return StringLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng); }
<INTERP_STRING>\\.              { return StringLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng); }

<INTERP_STRING>"{"              { return BracesLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACES); }
<INTERP_STRING>"}"              { return BracesLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACES); }

<INTERP_STRING>{identifier}     { return IdentifierLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng, IDENTIFIER); }

<INTERP_STRING>\"               { EndInterpolatedStringLexemeAction(createLexicalAnalyzerContext());                                   BEGIN(INITIAL); }

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
{comment} {IgnoredLexemeAction(createLexicalAnalyzerContext());}
{decimal}				    		{ return DecimalLexemeAction(createLexicalAnalyzerContext()); }
[+-]?[[:digit:]]+					{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }

"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"float"                             { return FloatKeywordLexemeAction(createLexicalAnalyzerContext()); }

"int"                               { BEGIN(INT_ASSIGNMENT); return IntKeywordLexemeAction(createLexicalAnalyzerContext()); }
<INT_ASSIGNMENT>{identifier}        { return IdentifierLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng, INT_ID);  BEGIN(INITIAL); }
<INT_ASSIGNMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<INT_ASSIGNMENT>.					{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }



"vector"                            { return VectorKeywordLexemeAction(createLexicalAnalyzerContext()); }

"%"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MOD); }

">="                                { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), GEQ); }
"<="                                { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), LEQ); }                        
">"                                 { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), GT); }
"<"                                 { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), LT); }
"=="                                { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), EQ); }
"!="                                { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), NEQ); }
"and"                               { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), AND); }
"or"                                { return BinaryOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }
"not"                               { return UnaryOperatorLexemeAction(createLexicalAnalyzerContext(), NOT); }

"="                                 { return AssignmentLexemeAction(createLexicalAnalyzerContext()); }
"log"                               { return LogLexemeAction(createLexicalAnalyzerContext()); }

","                                 { return CommaLexemeAction(createLexicalAnalyzerContext()); }
";"                                  { return SemicolonLexemeAction(createLexicalAnalyzerContext()); }
"."                                 { return DotLexemeAction(createLexicalAnalyzerContext()); }
"x"                                 { return XParamLexemeAction(createLexicalAnalyzerContext()); }
"y"                                 { return YParamLexemeAction(createLexicalAnalyzerContext()); }
":"                                 { return ColonLexemeAction(createLexicalAnalyzerContext()); }

"import"                            { return ImportLexemeAction(createLexicalAnalyzerContext()); }
"if"                                { return IfLexemeAction(createLexicalAnalyzerContext()); }
"else"                              { return ElseLexemeAction(createLexicalAnalyzerContext()); }
"in"                                { return InLexemeAction(createLexicalAnalyzerContext()); }
"for"                               { return ForLexemeAction(createLexicalAnalyzerContext()); }

"{"									{ return BracesLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACES); }
"}"									{ return BracesLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACES); }
"["									{ return BracketsLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACKETS); }
"]"									{ return BracketsLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACKETS); }



"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }




{identifier}                        { return IdentifierLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng, IDENTIFIER); }
{string}                            { return StringLexemeAction(createLexicalAnalyzerContext(), yytext, yyleng); }

[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }


.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }



%%

#include "FlexExport.h"


/**
 * Moví lo que pensaba que no tenía sentido
 * 
 * 
 * NOOOOO ESTO NOOOO PLZ:
 * "+="                                { return PlusEqualsLexemeAction(createLexicalAnalyzerContext()); }
 * "-="                                { return MinusEqualsLexemeAction(createLexicalAnalyzerContext()); }
 * "*="                                { return TimesEqualsLexemeAction(createLexicalAnalyzerContext()); }
 * "/="                                { return DivEqualsLexemeAction(createLexicalAnalyzerContext()); }
 * "%="                                { return ModEqualsLexemeAction(createLexicalAnalyzerContext()); }
 * 
 * TODO: Ojo que faltan parsear los floats
 */