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
	ExpressionList * expression_list;
	Constant * constant;
	FloatExpression * float_expression;
	FloatFactor * float_factor;
	Vector * vector;
	Program * program;
	// TODO: Descomentar una vez que esté todo en el AbstractSyntaxTree

	Sentences * sentences;
	Sentence * sentence;

	Block * block;
	Interval * interval;
	BoolExpression * bool_expression;
	BoolFactor * bool_factor;

	IntegerExpression * integer_expression;
	IntegerFactor * integer_factor;
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
%destructor { releaseFloatExpression($$); } <float_expression>
%destructor { releaseFloatFactor($$); } <float_factor>
%destructor { releaseSentences($$); } <sentences>
%destructor { releaseSentence($$); } <sentence>
%destructor { releaseBlock($$); } <block>
%destructor { releaseInterval($$); } <interval>
%destructor { releaseBoolExpression($$); } <bool_expression>
%destructor { releaseBoolFactor($$); } <bool_factor>

%destructor { releaseIntegerExpression($$); } <integer_expression>
%destructor { releaseIntegerFactor($$); } <integer_factor>





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
%token <token> LOG

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
%type <float_expression> float_expression
%type <float_factor> float_factor

%type <vector> vector
%type <program> program

/** NUESTROS NO-TERMINALES */

%type <sentences> sentences
%type <sentence> sentence
%type <expression_list> expression_list

%type <block> block
%type <interval> interval
%type <bool_expression> bool_expression
%type <bool_factor> bool_factor

%type <integer_expression> integer_expression
%type <integer_factor> integer_factor






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


sentences: sentences sentence														{ $$ = SentencesSemanticAction($1, $2); }
	| %empty                                                            			{ $$ = EmptySentencesSemanticAction(); }
	;

sentence: IDENTIFIER ASSIGN float_expression SEMICOLON								{ $$ = AssignSentenceSemanticAction($1, $3); }
	| IDENTIFIER ASSIGN OPEN_BRACKETS expression_list CLOSE_BRACKETS SEMICOLON { $$ = AssignArraySentenceSemanticAction($1, $4); }
	/* | IDENTIFIER ASSIGN vector											// TODO: VECTOR */
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block					{ $$ = IfSentenceSemanticAction($3, $5); }
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block ELSE block		{ $$ = IfElseSentenceSemanticAction($3, $5, $7); }
	| FOR IDENTIFIER IN interval block												{ $$ = ForSentenceSemanticAction($2, $4, $5); }
	| IDENTIFIER OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS	SEMICOLON		{ $$ = FunctionSentenceSemanticAction($1, $3); }
	;

block: OPEN_BRACES sentences CLOSE_BRACES											{ $$ = BlockSemanticAction($2); }
	;

interval: OPEN_BRACKETS float_expression COLON float_expression CLOSE_BRACKETS		{ $$ = IntervalSemanticAction($2, $4); }
	| IDENTIFIER 																	{ $$ = IntervalIdentifierSemanticAction($1); }
	;

bool_expression: float_expression GEQ float_expression								{ $$ = BoolExpressionSemanticAction($1, $3, GREATER_OR_EQUAL); }
	| float_expression LEQ float_expression											{ $$ = BoolExpressionSemanticAction($1, $3, LESS_OR_EQUAL); }
	| float_expression GT float_expression											{ $$ = BoolExpressionSemanticAction($1, $3, GREATER_THAN); }
	| float_expression LT float_expression											{ $$ = BoolExpressionSemanticAction($1, $3, LESS_THAN); }
	| float_expression EQ float_expression											{ $$ = BoolExpressionSemanticAction($1, $3, EQUAL_TO); }
	| float_expression NEQ float_expression											{ $$ = BoolExpressionSemanticAction($1, $3, NOT_EQUAL); }
	| bool_expression AND bool_expression											{ $$ = BoolBinaryExpressionSemanticAction($1, $3, AND_TYPE); }
	| bool_expression OR bool_expression											{ $$ = BoolBinaryExpressionSemanticAction($1, $3, OR_TYPE); }
	| NOT bool_expression															{ $$ = BoolUnaryExpressionSemanticAction($2, NOT_TYPE); }
	| bool_factor																	{ $$ = BoolFactorExpressionSemanticAction($1); }
	;

bool_factor: OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS						{ $$ = BoolExpressionFactorSemanticAction($2); }
	;



/* 
	Comentarios para seguir con el trabajo:
	TODO: Las operaciones con vectores NO deberían ser compatibles con las operaciones numéricas

	TODO: Si queremos aceptar operaciones EXCLUSIVAS para integers (/, %), deberíamos tener un
		"integer_expression" y un "numeric_expression" (o algo así) y no mezclar tipos.

 */

expression_list: expression_list[left] COMMA float_expression[right]				{ $$ = ExpressionListSemanticAction($left, $right); }
	| float_expression[right]				{ $$ = ExpressionListSemanticAction(NULL, $right); }
	| %empty													{ $$ = EmptyExpressionListSemanticAction(); }
	;

float_expression: float_expression[left] ADD float_expression[right]				{ $$ = FloatArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| float_expression[left] DIV float_expression[right]							{ $$ = FloatArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| float_expression[left] MUL float_expression[right]							{ $$ = FloatArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| float_expression[left] SUB float_expression[right]							{ $$ = FloatArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| float_factor																	{ $$ = FloatFactorExpressionSemanticAction($1); }
/*	| vector_expression DOT X
	| vector_expression DOT Y	falta en el flex
	| integer_expression
*/
	;

float_factor: OPEN_PARENTHESIS float_expression CLOSE_PARENTHESIS					{ $$ = FloatExpressionFactorSemanticAction($2); }
	| constant																		{ $$ = FloatConstantFactorSemanticAction($1); }
	| vector																		{ $$ = VectorFactorSemanticAction($1); }
	| OPEN_PARENTHESIS "float" CLOSE_PARENTHESIS integer_expression															{ $$ = IntegerToFloatSemanticAction($1); }
// El vector no debería ser un float_factor. Debería tener su propia categoría de vector_expression
	;


integer_expression: integer_expression[left] ADD integer_expression[right]			{ $$ = IntegerArithmeticExpressionSemanticAction($left, $right, INT_ADDITION); }
	| integer_expression[left] DIV integer_expression[right]						{ $$ = IntegerArithmeticExpressionSemanticAction($left, $right, INT_DIVISION); }
	| integer_expression[left] MOD integer_expression[right]						{ $$ = IntegerArithmeticExpressionSemanticAction($left, $right, INT_MODULUS); }
	| integer_expression[left] MUL integer_expression[right]						{ $$ = IntegerArithmeticExpressionSemanticAction($left, $right, INT_MULTIPLICATION); }
	| integer_expression[left] SUB integer_expression[right]						{ $$ = IntegerArithmeticExpressionSemanticAction($left, $right, INT_SUBTRACTION); }
	| integer_factor																{ $$ = IntegerFactorExpressionSemanticAction($1); }
	;

integer_factor: OPEN_PARENTHESIS integer_expression CLOSE_PARENTHESIS				{ $$ = IntegerExpressionFactorSemanticAction($2); }
	| INTEGER																		{ $$ = IntegerConstantSemanticAction($1); }
	| IDENTIFIER																	{ $$ = IntegerIdentifierSemanticAction($1); }
	;

/*
vector_expression: vector_expression[left] ADD vector_expression[right]					{ $$ = VectorArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| vector_expression[left] DIV float_expression[right]						{ $$ = VectorArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| vector_expression[left] MUL float_expression[right]						{ $$ = VectorArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| vector_expression[left] SUB vector_expression[right]						{ $$ = VectorArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| float_factor														{ $$ = FloatFactorExpressionSemanticAction($1); }
	; */

	

constant: DECIMAL													{ $$ = DecimalConstantSemanticAction($1); }
	/* | INTEGER													{ $$ = IntegerConstantSemanticAction($1); } */
	/* | IDENTIFIER												{ $$ = IdentifierConstantSemanticAction($1); } */
	;




vector: OPEN_PARENTHESIS constant[left] COMMA constant[right] CLOSE_PARENTHESIS	{ $$ = VectorSemanticAction($left, $right); }
	;

// Deberíamos reemplazar la anterior por esta
/* vector: OPEN_PARENTHESIS float_expression[left] COMMA float_expression[right] CLOSE_PARENTHESIS	{ $$ = VectorSemanticAction($left, $right); }
	; */
	

%%
