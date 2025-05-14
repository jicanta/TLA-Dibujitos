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
		switch (constant->type) {
			case INTEGER_CONSTANT:
				// No need to free anything for integer constants
				break;
			case DECIMAL_CONSTANT:
				// No need to free anything for decimal constants
				break;
			case IDENTIFIER_CONSTANT:
				free((char *) constant->identifier); // Cast to char* to free the string
				break;
		}
		free(constant);
	}
}

void releaseExpression(Expression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case ADDITION:
			case DIVISION:
			case MULTIPLICATION:
			case SUBTRACTION:
				releaseExpression(expression->leftExpression);
				releaseExpression(expression->rightExpression);
				break;
			case FACTOR:
				releaseFactor(expression->factor);
				break;
		}
		free(expression);
	}
}

void releaseFactor(Factor * factor) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (factor != NULL) {
		switch (factor->type) {
			case CONSTANT:
				releaseConstant(factor->constant);
				break;
			case EXPRESSION:
				releaseExpression(factor->expression);
				break;
			case VECTOR:
				releaseVector(factor->vector);
				break;
		}
		free(factor);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (program != NULL) {
		releaseSentences(program->sentences);
		free(program);
	}
}

void releaseVector(Vector * vector) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (vector != NULL) {
		releaseConstant(vector->left);
		releaseConstant(vector->right);
		free(vector);
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
				releaseExpression(sentence->assignExpression);
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
		releaseExpression(interval->leftExpression);
		releaseExpression(interval->rightExpression);
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
				releaseExpression(boolExpression->leftExpression);
				releaseExpression(boolExpression->rightExpression);
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