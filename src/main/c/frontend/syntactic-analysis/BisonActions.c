#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

Constant * IntegerConstantSemanticAction(const int integer) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->integer = integer;
	constant->type = INTEGER_CONSTANT;
	return constant;
}

Constant * IdentifierConstantSemanticAction(const char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->identifier = identifier;
	constant->type = IDENTIFIER_CONSTANT;
	return constant;
}


Constant * DecimalConstantSemanticAction(const float decimal) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->decimal = decimal;
	constant->type = DECIMAL_CONSTANT;
	return constant;
}

FloatExpression * FloatArithmeticExpressionSemanticAction(FloatExpression * leftFloatExpression, FloatExpression * rightFloatExpression, FloatExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatExpression * floatExpression = calloc(1, sizeof(FloatExpression));
	floatExpression->leftFloatExpression = leftFloatExpression;
	floatExpression->rightFloatExpression = rightFloatExpression;
	floatExpression->type = type;
	return floatExpression;
}

FloatExpression * FloatFactorExpressionSemanticAction(FloatFactor * floatFactor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatExpression * floatExpression = calloc(1, sizeof(FloatExpression));
	floatExpression->floatFactor = floatFactor;
	floatExpression->type = FACTOR;
	return floatExpression;
}

FloatExpression * VectorDotExpressionSemanticAction(VectorExpression * vectorExpression, FloatExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatExpression * floatExpression = calloc(1, sizeof(FloatExpression));
	floatExpression->vectorExpression = vectorExpression;
	floatExpression->type = type;
	return floatExpression;
}

FloatFactor * FloatConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatFactor * floatFactor = calloc(1, sizeof(FloatFactor));
	floatFactor->constant = constant;
	floatFactor->type = CONSTANT;
	return floatFactor;
}

FloatFactor * FloatExpressionFactorSemanticAction(FloatExpression * floatExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatFactor * floatFactor = calloc(1, sizeof(FloatFactor));
	floatFactor->floatExpression = floatExpression;
	floatFactor->type = EXPRESSION;
	return floatFactor;
}


Vector * VectorSemanticAction(Constant * left, Constant * right) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Vector * vector = calloc(1, sizeof(Vector));
	vector->left = left;
	vector->right = right;
	return vector;
}

Program * SentencesProgramSemanticAction(CompilerState * compilerState, Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->sentences = sentences;
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
	return program;
}


// NUESTRAS:

Sentences * EmptySentencesSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Sentences * sentences = calloc(1, sizeof(Sentences));
    // Initialize fields to represent an empty state, if necessary.
    return sentences;
}

ExpressionList * ExpressionListSemanticAction(ExpressionList * expressionList, FloatExpression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionList * expressionList_ret = calloc(1, sizeof(ExpressionList));
	expressionList_ret->expression = expression;
	expressionList_ret->next = expressionList;
	return expressionList_ret;
}

ExpressionList * EmptyExpressionListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ExpressionList * expressionList = calloc(1, sizeof(ExpressionList));
    // Initialize fields to represent an empty state, if necessary.
    return expressionList;
}

Sentence * FunctionSentenceSemanticAction(char * identifier, ExpressionList * functionArguments) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->functionIdentifier = identifier;
	sentence->functionArguments = functionArguments;
	sentence->type = FUNCTION_SENTENCE;
	return sentence;
}

Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentences * sentences_ret = calloc(1, sizeof(Sentences));
	sentences_ret->sentences = sentences;
	sentences_ret->sentence = sentence;
	return sentences_ret;
}

Sentence * AssignFloatSentenceSemanticAction(char * identifier, FloatExpression * floatExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignFloatIdentifier = identifier;
	sentence->assignFloatExpression = floatExpression;
	sentence->type = ASSIGN_SENTENCE;
	return sentence;
}

Sentence * AssignIntegerSentenceSemanticAction(char * identifier, IntegerExpression * integerExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignIntegerIdentifier = identifier;
	sentence->assignIntegerExpression = integerExpression;
	sentence->type = ASSIGN_INT_SENTENCE;
	return sentence;
}
Sentence * AssignVectorSentenceSemanticAction(char * identifier, VectorExpression * vectorExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignVectorIdentifier = identifier;
	sentence->assignVectorExpression = vectorExpression;
	sentence->type = ASSIGN_VECTOR_SENTENCE;
	return sentence;
}

Sentence * AssignArraySentenceSemanticAction(char * identifier, ExpressionList * expressionList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignArrayIdentifier = identifier;
	sentence->arrayExpressionList = expressionList;
	sentence->type = ASSIGN_ARRAY_SENTENCE;
	return sentence;
}

Sentence * IfSentenceSemanticAction(BoolExpression * boolExpression, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->ifBoolExpression = boolExpression;
	sentence->ifBlock = block;
	sentence->type = IF_SENTENCE;
	return sentence;
}

Sentence * ForSentenceSemanticAction(char * identifier, Interval * interval, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->forIdentifier = identifier;
	sentence->forInterval = interval;
	sentence->forBlock = block;
	sentence->type = FOR_SENTENCE;
	return sentence;
}

Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->ifElseBoolExpression = boolExpression;
	sentence->leftIfElseBlock = leftBlock;
	sentence->rightIfElseBlock = rightBlock;
	sentence->type = IF_ELSE_SENTENCE;
	return sentence;
}

Block * BlockSemanticAction(Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * block = calloc(1, sizeof(Block));
	block->sentences = sentences;
	return block;
}

Interval * IntervalSemanticAction(FloatExpression * leftFloatExpression, FloatExpression * rightFloatExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Interval * interval = calloc(1, sizeof(Interval));
	interval->leftFloatExpression = leftFloatExpression;
	interval->rightFloatExpression = rightFloatExpression;
	interval->type = RANGE_INTERVAL;
	return interval;
}

Interval * IntervalIdentifierSemanticAction(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Interval * interval = calloc(1, sizeof(Interval));
	interval->identifier = identifier;
	interval->type = IDENTIFIER_INTERVAL;
	return interval;
}

BoolExpression * BoolExpressionSemanticAction(FloatExpression * leftFloatExpression, FloatExpression * rightFloatExpression, BoolExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolExpression * boolExpression = calloc(1, sizeof(BoolExpression));
	boolExpression->leftFloatExpression = leftFloatExpression;
	boolExpression->rightFloatExpression = rightFloatExpression;
	boolExpression->type = type;
	return boolExpression;
}

BoolExpression * BoolBinaryExpressionSemanticAction(BoolExpression * leftBoolExpression, BoolExpression * rightBoolExpression, BoolExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolExpression * boolExpression = calloc(1, sizeof(BoolExpression));
	boolExpression->leftBoolExpression = leftBoolExpression;
	boolExpression->rightBoolExpression = rightBoolExpression;
	boolExpression->type = type;
	return boolExpression;
}


BoolExpression * BoolUnaryExpressionSemanticAction(BoolExpression * boolExpression, BoolExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolExpression * boolExpression_ret = calloc(1, sizeof(BoolExpression));
	boolExpression_ret->boolExpression = boolExpression;
	boolExpression_ret->type = type;
	return boolExpression_ret;
}

BoolExpression * BoolFactorExpressionSemanticAction(BoolFactor * boolFactor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolExpression * boolExpression = calloc(1, sizeof(BoolExpression));
	boolExpression->boolFactor = boolFactor;
	boolExpression->type = BOOL_FACTOR;
	return boolExpression;
}

BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolFactor * boolFactor = calloc(1, sizeof(BoolFactor));
	boolFactor->boolExpression = boolExpression;
	return boolFactor;
}

IntegerExpression * IntegerArithmeticExpressionSemanticAction(IntegerExpression * leftIntegerExpression, IntegerExpression * rightIntegerExpression, IntegerExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntegerExpression * integerExpression = calloc(1, sizeof(IntegerExpression));
	integerExpression->leftIntegerExpression = leftIntegerExpression;
	integerExpression->rightIntegerExpression = rightIntegerExpression;
	integerExpression->type = type;
	return integerExpression;
}


FloatFactor * IntegerToFloatFactorSemanticAction(IntegerExpression * integerExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatFactor * floatFactor = calloc(1, sizeof(FloatFactor));
	floatFactor->integerExpression = integerExpression;
	floatFactor->type = INTEGER_TO_FLOAT;
	return floatFactor;
}

IntegerExpression * IntegerFactorExpressionSemanticAction(IntegerFactor * integerFactor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntegerExpression * integerExpression = calloc(1, sizeof(IntegerExpression));
	integerExpression->integerFactor = integerFactor;
	integerExpression->type = INT_FACTOR;
	return integerExpression;
}

IntegerFactor * IntegerExpressionFactorSemanticAction(IntegerExpression * integerExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntegerFactor * integerFactor = calloc(1, sizeof(IntegerFactor));
	integerFactor->integerExpression = integerExpression;
	integerFactor->type = INT_EXPRESSION;
	return integerFactor;
}
IntegerFactor * IntegerConstantFactorSemanticAction(int integer) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntegerFactor * integerFactor = calloc(1, sizeof(IntegerFactor));
	integerFactor->integer = integer;
	integerFactor->type = INT_CONSTANT;
	return integerFactor;
}
IntegerFactor * IntegerIdentifierFactorSemanticAction(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntegerFactor * integerFactor = calloc(1, sizeof(IntegerFactor));
	integerFactor->identifier = identifier;
	integerFactor->type = INT_IDENTIFIER;
	return integerFactor;
}

VectorExpression * VectorArithmeticExpressionSemanticAction(VectorExpression * leftVectorExpression, VectorExpression * rightVectorExpression, VectorExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorExpression * vectorExpression = calloc(1, sizeof(VectorExpression));
	vectorExpression->leftVectorExpression = leftVectorExpression;
	vectorExpression->rightVectorExpression = rightVectorExpression;
	vectorExpression->type = type;
	return vectorExpression;
}
VectorExpression * VectorFloatArithmeticExpressionSemanticAction(VectorExpression * leftVectorExpression, FloatExpression * rightFloatExpression, VectorExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorExpression * vectorExpression = calloc(1, sizeof(VectorExpression));
	vectorExpression->vectorExpression = leftVectorExpression;
	vectorExpression->floatExpression = rightFloatExpression;
	vectorExpression->type = type;
	return vectorExpression;
}
VectorExpression * VectorFactorExpressionSemanticAction(VectorFactor * vectorFactor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorExpression * vectorExpression = calloc(1, sizeof(VectorExpression));
	vectorExpression->vectorFactor = vectorFactor;
	vectorExpression->type = VEC_FACTOR;
	return vectorExpression;
}

VectorFactor * VectorExpressionFactorSemanticAction(VectorExpression * vectorExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorFactor * vectorFactor = calloc(1, sizeof(VectorFactor));
	vectorFactor->vectorExpression = vectorExpression;
	vectorFactor->type = VEC_EXPRESSION;
	return vectorFactor;
}

VectorFactor * VectorIdentifierFactorSemanticAction(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorFactor * vectorFactor = calloc(1, sizeof(VectorFactor));
	vectorFactor->identifier = identifier;
	vectorFactor->type = VEC_IDENTIFIER;
	return vectorFactor;
}

VectorFactor * VectorFactorSemanticAction(Vector * vector) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	VectorFactor * vectorFactor = calloc(1, sizeof(VectorFactor));
	vectorFactor->vector = vector;
	vectorFactor->type = VEC_VECTOR;
	return vectorFactor;
}