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
%x INTERP_EXPR

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

digit              [0-9]
decimal            [+-]?(([0-9]+\.[0-9]*)|(\.[0-9]+))
hex_integer        0x[0-9a-fA-F]+
integer            [+-]?{digit}+
char               [a-zA-Z_]
identifier         {char}({char}|{digit})*
single_comment     \/\/[^\n]*
%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(yytext); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(yytext); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(yytext); }

{single_comment}                    { IgnoredLexemeAction(yytext); }

{decimal}				    		{ return DecimalLexemeAction(yytext); }
{integer}	        				{ return IntegerLexemeAction(yytext); }
{hex_integer}	        		    { return HexIntegerLexemeAction(yytext); }

\"                                  { BEGIN(INTERP_STRING); return BeginInterpolatedStringLexemeAction(); }
<INTERP_STRING>"{"                  { BEGIN(INTERP_EXPR); return BracesLexemeAction(OPEN_BRACES, yytext); }
<INTERP_STRING>[^\\"{}]+            { return StringLexemeAction(yytext, yyleng); }
<INTERP_STRING>\\.                  { return StringLexemeAction(yytext, yyleng); }
<INTERP_STRING>"}"                  { return BracesLexemeAction(CLOSE_BRACES, yytext); }
<INTERP_STRING>\"                   { BEGIN(INITIAL); return EndInterpolatedStringLexemeAction(); }
<INTERP_EXPR>{identifier}           { BEGIN(INTERP_STRING); return IdentifierLexemeAction(yytext, yyleng); }
<INTERP_EXPR>"}"                    { BEGIN(INTERP_STRING); return BracesLexemeAction(CLOSE_BRACES, yytext); }

"-"									{ return ArithmeticOperatorLexemeAction(SUB, yytext); }
"*"									{ return ArithmeticOperatorLexemeAction(MUL, yytext); }
"/"									{ return ArithmeticOperatorLexemeAction(DIV, yytext); }
"+"									{ return ArithmeticOperatorLexemeAction(ADD, yytext); }
"%"									{ return ArithmeticOperatorLexemeAction(MOD, yytext); }

">="                                { return BinaryOperatorLexemeAction(GEQ, yytext); }
"<="                                { return BinaryOperatorLexemeAction(LEQ, yytext); }                        
">"                                 { return BinaryOperatorLexemeAction(GT, yytext); }
"<"                                 { return BinaryOperatorLexemeAction(LT, yytext); }
"=="                                { return BinaryOperatorLexemeAction(EQ, yytext); }
"!="                                { return BinaryOperatorLexemeAction(NEQ, yytext); }
"and"                               { return BinaryOperatorLexemeAction(AND, yytext); }
"or"                                { return BinaryOperatorLexemeAction(OR, yytext); }
"not"                               { return UnaryOperatorLexemeAction(NOT, yytext); }

"="                                 { return AssignmentLexemeAction(); }
"log"                               { return LogLexemeAction(); }

","                                 { return CommaLexemeAction(); }
";"                                 { return SemicolonLexemeAction(); }
"."                                 { return DotLexemeAction(); }
"x"                                 { return XParamLexemeAction(); }
"y"                                 { return YParamLexemeAction(); }
":"                                 { return ColonLexemeAction(); }

"import"                            { return ImportLexemeAction(); }
"if"                                { return IfLexemeAction(); }
"else"                              { return ElseLexemeAction(); }
"in"                                { return InLexemeAction(); }
"for"                               { return ForLexemeAction(); }

"{"									{ return BracesLexemeAction(OPEN_BRACES, yytext); }
"}"									{ return BracesLexemeAction(CLOSE_BRACES, yytext); }
"["									{ return BracketsLexemeAction(OPEN_BRACKET, yytext); }
"]"									{ return BracketsLexemeAction(CLOSE_BRACKET, yytext); }


"("									{ return ParenthesisLexemeAction(OPEN_PARENTHESIS, yytext); }
")"									{ return ParenthesisLexemeAction(CLOSE_PARENTHESIS, yytext); }




{identifier}                        { return IdentifierLexemeAction(yytext, yyleng); }

[[:space:]]+						{ IgnoredLexemeAction(yytext); }


.									{ return UnknownLexemeAction(yytext); }



%%

#include "FlexExport.h"
