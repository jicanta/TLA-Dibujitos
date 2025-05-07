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

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

Expression * ArithmeticExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

Expression * FactorExpressionSemanticAction(Factor * factor) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->factor = factor;
	expression->type = FACTOR;
	return expression;
}

Factor * ConstantFactorSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->constant = constant;
	factor->type = CONSTANT;
	return factor;
}

Factor * ExpressionFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = EXPRESSION;
	return factor;
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

Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentences * sentences_ret = calloc(1, sizeof(Sentences));
	sentences_ret->sentences = sentences;
	sentences_ret->sentence = sentence;
	return sentences_ret;
}

Sentence * AssignSentenceSemanticAction(char * identifier, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignIdentifier = identifier;
	sentence->assignExpression = expression;
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

Interval * IntervalSemanticAction(Expression * leftExpression, Expression * rightExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Interval * interval = calloc(1, sizeof(Interval));
	interval->leftExpression = leftExpression;
	interval->rightExpression = leftExpression;
	return interval;
}

BoolExpression * BoolExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BoolExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolExpression * boolExpression = calloc(1, sizeof(BoolExpression));
	boolExpression->leftExpression = leftExpression;
	boolExpression->rightExpression = rightExpression;
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
	return boolExpression;
}

BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolFactor * boolFactor = calloc(1, sizeof(BoolFactor));
	boolFactor->boolExpression = boolExpression;
	return boolFactor;
}














