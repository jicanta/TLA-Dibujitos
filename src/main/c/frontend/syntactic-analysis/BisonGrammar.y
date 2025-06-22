%{

#include "BisonActions.h"
#include "AbstractSyntaxTree.h"



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
	Expressions * expressions;
	Expression * expression;
	Factor * factor;
	Vector * vector;
	Program * program;

	StringPartList * string_part_list;
	StringPart * string_part;
	Sentences * sentences;
	Sentence * sentence;

	Block * block;
	BoolExpression * bool_expression;
	BoolFactor * bool_factor;
	Array * array;

}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldnt used
 * this approach for the AST root node ("program" non-terminal, in this
 * grammar), or it will drop the entire tree even if the parse succeeds.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseSentences($$); } <sentences>
%destructor { releaseSentence($$); } <sentence>
%destructor { releaseBlock($$); } <block>

%destructor { releaseBoolExpression($$); } <bool_expression>
%destructor { releaseBoolFactor($$); } <bool_factor>

%destructor { releaseVector($$); } <vector>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseExpressions($$); } <expressions>
%destructor { releaseExpressionList($$); } <expression_list>
%destructor { releaseStringPartList($$); } <string_part_list>
%destructor { releaseStringPart($$); } <string_part>
%destructor { releaseArray($$); } <array>





/** Terminals. */
%token <integer> INTEGER
%token <decimal> DECIMAL
%token <token> ADD
%token <token> CLOSE_PARENTHESIS
%token <token> DIV
%token <token> MUL
%token <token> OPEN_PARENTHESIS
%token <token> SUB
%token <token> BEGIN_STRING
%token <token> END_STRING

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
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET

%token <token> COMMA
%token <token> SEMICOLON
%token <token> DOT
%token <token> X_PARAM
%token <token> Y_PARAM
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

%token <token> FLOAT_KEYWORD
%token <token> INT_KEYWORD
%token <token> VECTOR_KEYWORD

%token <token> UNKNOWN

/** Non-terminals. */
%type <vector> vector
%type <program> program

%type <sentences> sentences
%type <sentence> sentence
%type <expression_list> expression_list
%type <string_part_list> string_part_list
%type <string_part> string_part

%type <block> block
%type <bool_expression> bool_expression
%type <bool_factor> bool_factor


%type <expression> expression
%type <factor> factor
%type <expressions> expressions
%type <array> array





/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV
%left MOD
%left DOT

%left AND OR

%left NOT

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: sentences																	{ $$ = SentencesProgramSemanticAction(currentCompilerState(), $1); }
	;


sentences: sentences sentence 														{ 
		if($2 == NULL) {
			YYABORT;
		}
		$$ = SentencesSemanticAction($1, $2); 
	}
	| %empty                                                            			{ $$ = EmptySentencesSemanticAction(); }
	;

sentence: IDENTIFIER ASSIGN expression SEMICOLON 									{ $$ = AssignSentenceSemanticAction($1, $3); }
	| IDENTIFIER OPEN_BRACKET CLOSE_BRACKET ASSIGN array SEMICOLON 					{ $$ = AssignArraySentenceSemanticAction($1, $5); }
	| IDENTIFIER OPEN_BRACKET expression CLOSE_BRACKET ASSIGN expression SEMICOLON  { $$ = AssignArrayElementSentenceSemanticAction($1, $3, $6); }
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block					{ $$ = IfSentenceSemanticAction($3, $5); }
	| IF OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS block ELSE block		{ $$ = IfElseSentenceSemanticAction($3, $5, $7); }
	| FOR IDENTIFIER IN array {
		if(!InsertForLoopIterator($2, $4)) {
			YYABORT;
		}
	} block																			{ $$ = ForSentenceSemanticAction($2, $4, $6); }
	| IDENTIFIER OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS	SEMICOLON		{ $$ = FunctionSentenceSemanticAction($1, $3); }
	| LOG BEGIN_STRING string_part_list END_STRING SEMICOLON						{ $$ = LogSentenceSemanticAction($3); }
	| IDENTIFIER DOT X_PARAM ASSIGN expression SEMICOLON							{ $$ = AssignVectorComponentSemanticAction($1, $5, GET_X); }
	| IDENTIFIER DOT Y_PARAM ASSIGN expression SEMICOLON							{ $$ = AssignVectorComponentSemanticAction($1, $5, GET_Y); }
	| IMPORT BEGIN_STRING string_part_list END_STRING SEMICOLON						{ $$ = ImportSentenceSemanticAction($3); }
	;

string_part_list
    : string_part string_part_list 													{ $$ = appendStringPartList($2, $1); }
    | string_part																	{ $$ = createStringPartList($1); }
    ;

string_part
    : STRING																		{ $$ = createStringSegment($1); }
    | OPEN_BRACES IDENTIFIER CLOSE_BRACES											{ $$ = createStringInterpolation($2); }
    ;

block: OPEN_BRACES {
		StartScope();
	} sentences CLOSE_BRACES														{ $$ = BlockSemanticAction($3); }
	;

array: OPEN_BRACKET expression_list CLOSE_BRACKET									{ $$ = BasicArraySemanticAction($2); }
	| OPEN_BRACKET expression COLON expression CLOSE_BRACKET						{ $$ = IntervalArraySemanticAction($2, $4); }
	| IDENTIFIER																	{ $$ = IdentifierArraySemanticAction($1); }
	;

expression_list: expressions														{ $$ = FilledExpressionListSemanticAction($1); }		
	| %empty																		{ $$ = EmptyExpressionListSemanticAction(); }
	;

bool_expression: expression GEQ expression											{ $$ = BoolComparisonExpressionSemanticAction($1, $3, GREATER_OR_EQUAL); }
	| expression LEQ expression														{ $$ = BoolComparisonExpressionSemanticAction($1, $3, LESS_OR_EQUAL); }
	| expression GT expression														{ $$ = BoolComparisonExpressionSemanticAction($1, $3, GREATER_THAN); }
	| expression LT expression														{ $$ = BoolComparisonExpressionSemanticAction($1, $3, LESS_THAN); }
	| expression EQ expression														{ $$ = BoolComparisonExpressionSemanticAction($1, $3, EQUAL_TO); }
	| expression NEQ expression														{ $$ = BoolComparisonExpressionSemanticAction($1, $3, NOT_EQUAL); }
	| bool_expression AND bool_expression											{ $$ = BoolBinaryExpressionSemanticAction($1, $3, AND_TYPE); }
	| bool_expression OR bool_expression											{ $$ = BoolBinaryExpressionSemanticAction($1, $3, OR_TYPE); }
	| NOT bool_expression															{ $$ = BoolUnaryExpressionSemanticAction($2, NOT_TYPE); }
	| bool_factor																	{ $$ = BoolFactorExpressionSemanticAction($1); }
	;

bool_factor: OPEN_PARENTHESIS bool_expression CLOSE_PARENTHESIS						{ $$ = BoolExpressionFactorSemanticAction($2); }
	;


expressions: expression COMMA expressions											{ $$ = ExpressionsSemanticAction($3, $1); }
	| expression																	{ $$ = ExpressionsSemanticAction(NULL, $1); }
	;

expression: expression[left] ADD expression[right]									{ $$ = ArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| expression[left] DIV expression[right]										{ $$ = ArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| expression[left] MUL expression[right]										{ $$ = ArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| expression[left] SUB expression[right]										{ $$ = ArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| expression[left] MOD expression[right]										{ $$ = ArithmeticExpressionSemanticAction($left, $right, MODULUS); }
	| expression DOT X_PARAM														{ $$ = DotExpressionSemanticAction($1, GET_X); }
	| expression DOT Y_PARAM														{ $$ = DotExpressionSemanticAction($1, GET_Y); }
	| factor																		{ $$ = FactorExpressionSemanticAction($1); }
	| array OPEN_BRACKET expression CLOSE_BRACKET									{ 
		$$ = ArrayAccessExpressionSemanticAction($1, $3); 
		if($$ == NULL) {
			YYABORT;
		}
	}
	| IDENTIFIER OPEN_PARENTHESIS expression_list CLOSE_PARENTHESIS					{ $$ = FunctionExpressionSemanticAction($1, $3); }
	;

factor: IDENTIFIER 																	{ $$ = IdentifierFactorSemanticAction($1); }
	| INTEGER																		{ $$ = IntegerFactorSemanticAction($1); }
	| DECIMAL																		{ $$ = DecimalFactorSemanticAction($1); }
	| vector																		{ $$ = VectorFactorSemanticAction($1); }
	| OPEN_PARENTHESIS expression CLOSE_PARENTHESIS									{ $$ = ParenthesisFactorSemanticAction($2); }
	;

vector: OPEN_PARENTHESIS expression[left] COMMA expression[right] CLOSE_PARENTHESIS	{ $$ = VectorSemanticAction($left, $right); }
	;
%%
