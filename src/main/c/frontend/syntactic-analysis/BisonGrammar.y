%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	float decimal;
	Token token;
	char * string;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	Factor * factor;
	Vector * vector;
	Program * program;
	// TODO: Descomentar una vez que esté todo en el AbstractSyntaxTree

	Sentences * sentences;
	Sentence * sentence;

	Block * block;
	Interval * interval;
	BoolExpression * bool_expression;
	BoolFactor * bool_factor;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldnt used
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
%token <decimal> DECIMAL
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

%token <string> STRING
%token <string> IDENTIFIER










%token <token> UNKNOWN

/** Non-terminals. */
%type <constant> constant
%type <expression> expression
%type <factor> factor
%type <vector> vector
%type <program> program

/** NUESTROS NO-TERMINALES */

%type <sentences> sentences
%type <sentence> sentence

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

%left AND OR

%left NOT

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: sentences													{ $$ = SentencesProgramSemanticAction(currentCompilerState(), $1); }
	;

/* TODO: Faltan todas las funciones release
 * TODO: Faltan muchas reglas/producciones, especialmente las de los vectores y las
*/


sentences: sentences sentence SEMICOLON									{ $$ = SentencesSemanticAction($1, $2); }
	| %empty                                                            { $$ = EmptySentencesSemanticAction(); }
	;

sentence: IDENTIFIER ASSIGN expression									{ $$ = AssignSentenceSemanticAction($1, $3); }
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block		{ $$ = IfSentenceSemanticAction($3, $5); }
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block ELSE block		{ $$ = IfElseSentenceSemanticAction($3, $5, $7); }
	| FOR IDENTIFIER IN interval block									{ $$ = ForSentenceSemanticAction($2, $4, $5); }
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




/* 
	Comentarios para seguir con el trabajo:
	TODO: Renombrar:
		expression -> numeric_expression (de floats y integers)
		factor -> numeric_factor
		constant -> numeric_constant (o directamente "number")

	TODO: Las operaciones con vectores NO deberían ser compatibles con las operaciones numéricas

	TODO: Si queremos aceptar operaciones EXCLUSIVAS para integers (/, %), deberíamos tener un
		"integer_expression" y un "numeric_expression" (o algo así) y no mezclar tipos.

	TODO: Para que los vectores puedan tener cualquier expresión numérica, hay que poner 
		"expression" en vez de "constant"
	
	TODO: Hay que sacar "vector" de "factor" porque esto hace que floats y vectores sean compatibles (pero no lo son)
 */

expression: expression[left] ADD expression[right]					{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]						{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| factor														{ $$ = FactorExpressionSemanticAction($1); }
	;

factor: OPEN_PARENTHESIS expression CLOSE_PARENTHESIS				{ $$ = ExpressionFactorSemanticAction($2); }
	| constant														{ $$ = ConstantFactorSemanticAction($1); }
	| vector														{ $$ = VectorFactorSemanticAction($1); }
	;

constant: INTEGER													{ $$ = IntegerConstantSemanticAction($1); }
	| DECIMAL													{ $$ = DecimalConstantSemanticAction($1); }
	| IDENTIFIER												{ $$ = IdentifierConstantSemanticAction($1); }
	;

vector: OPEN_PARENTHESIS constant[left] COMMA constant[right] CLOSE_PARENTHESIS	{ $$ = VectorSemanticAction($left, $right); }
	;

%%
