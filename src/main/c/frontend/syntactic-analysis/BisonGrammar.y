%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	Token token;
	char * string;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Program * program;
	// TODO: Descomentar una vez que esté todo en el AbstractSyntaxTree

	Sentences * sentences;
	Sentence * sentence;
	IfSentence * if_sentence;
	IfElseSentence * if_else_sentence;
	ForSentence * for_sentence;
	AssignSentence * assign_sentence;
	Block * block;
	Interval * interval;
	BoolExpression * bool_expression;
	BoolFactor * bool_factor;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>

/** Terminals. */
%token <integer> INTEGER
%token <token> ADD
%token <token> CLOSE_PARENTHESIS
%token <token> DIV
%token <token> MUL
%token <token> OPEN_PARENTHESIS
%token <token> SUB

/** NUESTROS TERMINALES */
%token <token> MOD

%token <token> GEQ
%token <token> LEQ
%token <token> GT
%token <token> LT
%token <token> EQ
%token <token> NEQ
%token <token> AND
%token <token> OR
%token <token> NOT

%token <token> OPEN_BRACES
%token <token> CLOSE_BRACES
%token <token> OPEN_BRACKETS
%token <token> CLOSE_BRACKETS

%token <token> COMMA
%token <token> SEMICOLON
%token <token> DOT
%token <token> COLON

%token <token> IMPORT

%token <token> ASSIGN

%token <token> IF
%token <token> ELSE
%token <token> IN
%token <token> FOR

%token <token> STRING
%token <token> IDENTIFIER










%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <program> program

/** NUESTROS NO-TERMINALES */

%type <sentences> sentences
%type <sentence> sentence
%type <if_sentence> if_sentence
%type <if_else_sentence> if_else_sentence
%type <for_sentence> for_sentence
%type <assign_sentence> assign_sentence
%type <block> block
%type <interval> interval
%type <bool_expression> bool_expression
%type <bool_factor> bool_factor






/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: expression													{ $$ = ExpressionProgramSemanticAction(currentCompilerState(), $1); }
	;

/* TODO: cambiar a program: sentences 
   TODO: descomentar. Ojo que todavía faltan funcionalidades como los vectores y los floats
   TODO: Manejar lógica en el AbstractSyntaxTree.h
*/


sentences: sentences sentence SEMICOLON									{ $$ = SentencesSemanticAction($1, $2); }
	;

/* TODO: Obs: Se podrían eliminar las transiciones unitarias y evitar un montón de código de más */
sentence: assign_sentence												{ $$ = TypeAssignSentenceSemanticAction($1); }
	| if_sentence														{ $$ = TypeIfSentenceSemanticAction($1); }
	| for_sentence														{ $$ = TypeForSentenceSemanticAction($1); }
	| if_else_sentence													{ $$ = TypeIfElseSentenceSemanticAction($1); }
	;

/* TODO: cambiar bool_expression por bool_factor */
if_sentence: IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block		{ $$ = IfSentenceSemanticAction($3, $5); }
	;

if_else_sentence: IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block ELSE block		{ $$ = IfElseSentenceSemanticAction($3, $5, $7); }
	;

for_sentence: FOR IDENTIFIER IN interval block							{ $$ = ForSentenceSemanticAction($2, $3, $4); }
	;

assign_sentence: IDENTIFIER ASSIGN expression							{ $$ = AssignSentenceSemanticAction($1, $3); }
	;

block: OPEN_BRACES sentences CLOSE_BRACES								{ $$ = BlockSemanticAction($2); }
	;

interval: OPEN_BRACKETS expression SEMICOLON expression CLOSE_BRACKETS	{ $$ = IntervalSemanticAction($2, $4); }
	;

bool_expression: expression GEQ expression								{ $$ = BoolExpressionSemanticAction($1, $3, GREATER_OR_EQUAL); }
	| expression LEQ expression											{ $$ = BoolExpressionSemanticAction($1, $3, LESS_OR_EQUAL); }
	| expression GT expression											{ $$ = BoolExpressionSemanticAction($1, $3, GREATER_THAN); }
	| expression LT expression											{ $$ = BoolExpressionSemanticAction($1, $3, LESS_THAN); }
	| expression EQ expression											{ $$ = BoolExpressionSemanticAction($1, $3, EQUAL_TO); }
	| expression NEQ expression											{ $$ = BoolExpressionSemanticAction($1, $3, NOT_EQUAL); }
	| bool_expression AND bool_expression								{ $$ = BoolBinaryExpressionSemanticAction($1, $3, AND_TYPE); }
	| bool_expression OR bool_expression								{ $$ = BoolBinaryExpressionSemanticAction($1, $3, OR_TYPE); }
	| NOT bool_expression												{ $$ = BoolUnaryExpressionSemanticAction($2, NOT_TYPE); }
	| bool_factor														{ $$ = BoolFactorExpressionSemanticAction($1); }
	;

bool_factor: OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS			{ $$ = BoolExpressionFactorSemanticAction($2); }
	;






/* Quizás conviene cambiar EXPRESSION por INTEGER o algo así */
expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	;

%%
