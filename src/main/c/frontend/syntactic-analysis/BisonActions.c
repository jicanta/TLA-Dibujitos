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

FloatFactor * VectorFactorSemanticAction(Vector * vector) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	FloatFactor * floatFactor = calloc(1, sizeof(Vector));
	floatFactor->vector = vector;
	floatFactor->type = VECTOR;
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

Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentences * sentences_ret = calloc(1, sizeof(Sentences));
	sentences_ret->sentences = sentences;
	sentences_ret->sentence = sentence;
	return sentences_ret;
}

Sentence * AssignSentenceSemanticAction(char * identifier, FloatExpression * floatExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignIdentifier = identifier;
	sentence->assignFloatExpression = floatExpression;
	sentence->type = ASSIGN_SENTENCE;
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














