#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		free(constant);
	}
}

void releaseFloatExpression(FloatExpression * floatExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (floatExpression != NULL) {
		switch (floatExpression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseFloatExpression(floatExpression->leftFloatExpression);
				releaseFloatExpression(floatExpression->rightFloatExpression);
				break;
			case FACTOR:
				releaseFloatFactor(floatExpression->floatFactor);
				break;
		}
		free(floatExpression);
	}
}

void releaseFloatFactor(FloatFactor * floatFactor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (floatFactor != NULL) {
		switch (floatFactor->type) {
			case CONSTANT:
				releaseConstant(floatFactor->constant);
				break;
			case EXPRESSION:
				releaseFloatExpression(floatFactor->floatExpression);
				break;
		}
		free(floatFactor);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseSentences(program->sentences);
		free(program);
	}
}

// TODO: Crear todas las funciones RELEASE
void releaseSentences(Sentences * sentences){
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (sentences != NULL) {
		releaseSentences(sentences->sentences); // Recursive
		releaseSentence(sentences->sentence);
		free(sentences);
	}
}

void releaseSentence(Sentence * sentence){
	if (sentence != NULL) {
		switch (sentence->type) {
			case IF_SENTENCE:
				releaseBoolExpression(sentence->ifBoolExpression);
				releaseBlock(sentence->ifBlock);
				break;
			case ASSIGN_SENTENCE:
				free(sentence->assignIdentifier);
				releaseFloatExpression(sentence->assignFloatExpression);
				break;
			case IF_ELSE_SENTENCE:
				releaseBoolExpression(sentence->ifBoolExpression);
				releaseBlock(sentence->leftIfElseBlock);
				releaseBlock(sentence->rightIfElseBlock);
				break;
			case FOR_SENTENCE:
				free(sentence->forIdentifier);
				releaseInterval(sentence->forInterval);
				releaseBlock(sentence->forBlock);
				break;
		}
		free(sentence);
	}
}

void releaseBlock(Block * block) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (block != NULL) {
		releaseSentences(block->sentences);
		free(block);
	}
}

void releaseInterval(Interval * interval) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (interval != NULL) {
		releaseFloatExpression(interval->leftFloatExpression);
		releaseFloatExpression(interval->rightFloatExpression);
		free(interval);
	}
}

void releaseBoolExpression(BoolExpression * boolExpression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (boolExpression != NULL) {
		switch (boolExpression->type) {
			case BOOL_FACTOR:
				releaseBoolFactor(boolExpression->boolFactor);
				break;
			case GREATER_OR_EQUAL: case LESS_OR_EQUAL:
			case GREATER_THAN: case LESS_THAN:
			case EQUAL_TO: case NOT_EQUAL:
				releaseFloatExpression(boolExpression->leftFloatExpression);
				releaseFloatExpression(boolExpression->rightFloatExpression);
				break;
			case AND_TYPE: case OR_TYPE:
				releaseBoolExpression(boolExpression->leftBoolExpression);
				releaseBoolExpression(boolExpression->rightBoolExpression);
				break;
			case NOT_TYPE:
				releaseBoolExpression(boolExpression->boolExpression);
				break;
		}
		free(boolExpression);
	}
}

void releaseBoolFactor(BoolFactor * boolFactor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (boolFactor != NULL) {
		releaseBoolExpression(boolFactor->boolExpression);
		free(boolFactor);
	}
}