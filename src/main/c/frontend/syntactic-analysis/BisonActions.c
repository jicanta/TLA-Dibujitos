#include "BisonActions.h"
#include "../../shared/CompilerState.h"

#include "BisonParser.h"
#include "../../backend/semantic-analysis/symbolTable.h"

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

// TODO: Delete. Right now, this is a dummy function to avoid compilation errors.
SymbolType typeOfExpressionDummy(Expression* expression) {
	return INTEGER_TYPE;
}
int intValue(Expression* expression) {
	return 0;
}
float floatValue(Expression* expression) {
	return 0.0f;
}
VectorData vectorValue(Expression* expression) {
	VectorData vector = {0.0f, 0.0f};
	return vector;
};

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

Expression * DotExpressionSemanticAction(Expression * expression, ExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * dotExpression = calloc(1, sizeof(Expression));
	dotExpression->expression = expression;
	dotExpression->type = type;
	return dotExpression;
}

Expression * ArrayAccessExpressionSemanticAction(Array * array, Expression * indexExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->array = array;
	expression->indexExpression = indexExpression;
	expression->type = ARRAY_ACCESS;

	// Semantic Analysis

	// TODO: Check if indexExpression is of type INTEGER_TYPE, not negative and less than the size of the array
	// const SymbolEntry symbolEntry = getSymbolEntry(currentCompilerState()->symbolTable, array->identifier);
	// BasicType* arrayElements = symbolEntry.value.arrayData.elements;
	// int arraySize = 0;
	// for (; arrayElements[arraySize].type != NULL_TYPE; arraySize++) {
	// 	// Count the number of elements in the array
	// }
	
	return expression;
}

Factor * IdentifierFactorSemanticAction(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	// Semantic Analysis
	const SymbolEntry symbolEntry = getSymbolEntry(currentCompilerState()->symbolTable, identifier);
	switch (symbolEntry.type) {
		case INTEGER_TYPE:
		case FLOAT_TYPE:
		case VECTOR_TYPE:
		break;
		case FUNCTION_TYPE:
			logError(_logger, "%s is a function", identifier);
			free(identifier);
			return NULL;
			case ARRAY_TYPE:
			logError(_logger, "%s is an array", identifier);
			free(identifier);
			return NULL;
			default:
			logError(_logger, "The identifier '%s' is not defined.", identifier);
			free(identifier);
			return NULL;
	}

	Factor * factor = calloc(1, sizeof(Factor));
	factor->identifier = identifier;
	factor->type = IDENTIFIER_FACTOR;
	return factor;
}

Factor * IntegerFactorSemanticAction(int integer) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->integerExpression = integer;
	factor->type = INTEGER_FACTOR;
	return factor;
}

Factor * DecimalFactorSemanticAction(float decimal) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->floatExpression = decimal;
	factor->type = DECIMAL_FACTOR;
	return factor;
}

Sentence * AssignArrayElementSentenceSemanticAction(char * identifier, Expression * indexExpression, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignArrayElemIdentifier = identifier;
	sentence->assignArrayIndexExpression = indexExpression;
	sentence->assignArrayElementExpression = expression;
	sentence->type = ASSIGN_ARRAY_ELEMENT_SENTENCE;
	return sentence;
	// TODO: Semantics needed
}

Factor * VectorFactorSemanticAction(Vector * vector) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->vector = vector;
	factor->type = VECTOR_FACTOR;
	return factor;
}

Factor * ParenthesisFactorSemanticAction(Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Factor * factor = calloc(1, sizeof(Factor));
	factor->expression = expression;
	factor->type = PARENTHESIS_FACTOR;
	return factor;
}


Vector * VectorSemanticAction(Expression * x, Expression * y) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Vector * vector = calloc(1, sizeof(Vector));
	vector->x = x;
	vector->y = y;
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

Sentences * EmptySentencesSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    Sentences * sentences = calloc(1, sizeof(Sentences));

    return sentences;
}

ExpressionList * FilledExpressionListSemanticAction(Expressions * expressions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ExpressionList * expressionList = calloc(1, sizeof(ExpressionList));
	expressionList->expressions = expressions;
	return expressionList;
}

ExpressionList * EmptyExpressionListSemanticAction() {
    _logSyntacticAnalyzerAction(__FUNCTION__);
    ExpressionList * expressionList = calloc(1, sizeof(ExpressionList));

    return expressionList;
}

Expressions * ExpressionsSemanticAction(Expressions * expressions, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expressions * expressions_ret = calloc(1, sizeof(Expressions));
	expressions_ret->next = expressions;
	expressions_ret->expression = expression;
	return expressions_ret;
}

int functionSemanticAnalyzerCheck(char * identifier, ExpressionList* functionArguments) {
	// Semantic Analysis
	const SymbolEntry symbolEntry = getSymbolEntry(currentCompilerState()->symbolTable, identifier);
	if (symbolEntry.type != FUNCTION_TYPE) {
		logError(_logger, "The identifier '%s' is not a function.", identifier);
		currentCompilerState()->succeed = false;
		return 0;
	}

	const SymbolType* fnParameterTypes = symbolEntry.value.functionData.dataTypes;

	// Case where the function has no parameters (ExpressionList should be empty)
	if (fnParameterTypes[0] == NULL_TYPE) {
		if (functionArguments->expressions == NULL) {
			return 1;
		}
		logError(_logger, "The function '%s' does not expect any parameters.", identifier);
		currentCompilerState()->succeed = false;
		return 0;
	}

	// Case where the function has parameters
	const Expressions* expressionIndexer = functionArguments->expressions;
	int i=0;
	for (i=0; fnParameterTypes[i] != NULL_TYPE; i++) {
		if (expressionIndexer == NULL) {
			logError(_logger, "The function '%s' is being called with not enough parameters.", identifier);
			currentCompilerState()->succeed = false;
			return 0;
		}

		if (fnParameterTypes[i] != typeOfExpression(expressionIndexer->expression)) {
			logError(_logger, "Invalid parameter in function '%s': Should be '%d'.", identifier, fnParameterTypes[i]);
			currentCompilerState()->succeed = false;
			return 0;
		}
		expressionIndexer = expressionIndexer->next;
	}
	// Additional operations if the function has infinite parameters
	if (symbolEntry.value.functionData.hasInfiniteParameters) {
		i--; // Adjust for the last parameter which can be infinite
		while (expressionIndexer != NULL) {
			if (fnParameterTypes[i] != typeOfExpression(expressionIndexer->expression)) {
				logError(_logger, "Invalid parameter in function '%s': Should be '%d'.", identifier, fnParameterTypes[i]);
				currentCompilerState()->succeed = false;
				return 0;
			}
			expressionIndexer = expressionIndexer->next;
		}
	}
	else {
		if (expressionIndexer != NULL) {
			logError(_logger, "The function '%s' is being called with too many parameters.", identifier);
			currentCompilerState()->succeed = false;
			return 0;
		}
	}
	return 1;
}

Sentence * FunctionSentenceSemanticAction(char * identifier, ExpressionList * functionArguments) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->functionIdentifier = identifier;
	sentence->functionArguments = functionArguments;
	sentence->type = FUNCTION_SENTENCE;

	return functionSemanticAnalyzerCheck(identifier, functionArguments)? sentence : NULL;
}

Sentences * SentencesSemanticAction(Sentences * sentences, Sentence * sentence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentences * sentences_ret = calloc(1, sizeof(Sentences));
	sentences_ret->sentences = sentences;
	sentences_ret->sentence = sentence;
	return sentences_ret;
}

Sentence * AssignSentenceSemanticAction(char * identifier, Expression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);

	// Semantic Analysis
	SymbolType expressionType = INVALID_TYPE;
	expressionType = typeOfExpression(expression);
	SymbolEntry symbolEntry = {
		.identifier = identifier,
		.type = expressionType, // Assuming the type of the expression is the type of the identifier
	};
	
	const SymbolEntry currentEntry = getSymbolEntry(currentCompilerState()->symbolTable, identifier);
	if (currentEntry.type != NULL_TYPE && currentEntry.type != expressionType) {
		logError(_logger, "The identifier '%s' has type mismatch", identifier);
		printSymbolEntry(currentEntry);
		printSymbolEntry(symbolEntry);
		puts("");
		return NULL;
	}
	// Insert the identifier into the symbol table
	switch (expressionType) {
		case INTEGER_TYPE:
			symbolEntry.value.integerData = intValueExpression(expression);
			break;
		case FLOAT_TYPE:
			symbolEntry.value.floatData = floatValueExpression(expression);
			break;
		case VECTOR_TYPE:
		// symbolEntry.value.vectorData = vectorValue(expression), //TODO: beware that the return value of calculate should be correct
			break;
		default:
			logError(_logger, "The identifier '%s' has invalid type", identifier);
			currentCompilerState()->succeed = false;
			free(identifier);
			releaseExpression(expression);
			return NULL;
	}
	insertSymbol(currentCompilerState()->symbolTable, symbolEntry);

	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignIdentifier = identifier;
	sentence->assignExpression = expression;
	sentence->type = ASSIGN_SENTENCE;

	return sentence;
}


Sentence * AssignArraySentenceSemanticAction(char * identifier, Array * array) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->assignArrayIdentifier = identifier;
	sentence->assignArray = array;
	sentence->type = ASSIGN_ARRAY_SENTENCE;

	// Check if the identifier is already defined in the symbol table
	const SymbolEntry currentEntry = getSymbolEntry(currentCompilerState()->symbolTable, identifier);
	if (currentEntry.type != NULL_TYPE) {
		logError(_logger, "The identifier '%s' is already defined.", identifier);
		currentCompilerState()->succeed = false;
		return NULL;
	}

	if (array->type == BASIC_ARRAY) {
		// Case where the array is empty
		if (array->expressionList->expressions == NULL) {
			logError(_logger, "The array '%s' is empty.", identifier);
			currentCompilerState()->succeed = false;
			return NULL;
		}

		BasicType* arrayElements = malloc(sizeof(SymbolEntry) * 100); // Assuming a maximum of 100 elements for simplicity

		Expressions* expressionsIndex = array->expressionList->expressions;
		SymbolType t = typeOfExpressionDummy(expressionsIndex->expression);

		// Check if all expressions in the array are of the same type
		for (int i=0; expressionsIndex != NULL; i++) {
			if (t != typeOfExpressionDummy(expressionsIndex->expression)) {
				logError(_logger, "The array '%s' has elements of different types.", identifier);
				currentCompilerState()->succeed = false;
				return NULL;
			}
			BasicType arrayElement = {
				.type = t, // Assuming the type of the array is the type of the elements
				.value.integerData = intValue(expressionsIndex->expression), // Assuming integer for simplicity
			};
			arrayElements[i] = arrayElement;
			expressionsIndex = expressionsIndex->next;
		}

		const SymbolEntry entry = {
			.identifier = identifier,
			.type = ARRAY_TYPE, // Assuming the type of the array is ARRAY_TYPE
			.value.arrayData = {
				.elements = arrayElements, // This will be filled later when the array is defined
				.dataType = array->type // Assuming the type of the array is the type of the identifier
			}
		};
		insertSymbol(currentCompilerState()->symbolTable, entry);
	} else if (array->type == IDENTIFIER_ARRAY) {

		SymbolEntry otherArrayEntry = getSymbolEntry(currentCompilerState()->symbolTable, array->identifier);
		if (otherArrayEntry.type == NULL_TYPE) {
			logError(_logger, "The identifier '%s' is not defined.", otherArrayEntry.identifier);
			currentCompilerState()->succeed = false;
			return NULL;
		}

		 SymbolEntry entry = {
			.identifier = identifier,
			.type = ARRAY_TYPE, // Assuming the type of the array is ARRAY_TYPE
			.value.arrayData = {
				.elements = otherArrayEntry.value.arrayData.elements, // Pointing to the same elements as the other array
				.dataType = array->type // Assuming the type of the array is the type of the identifier
			}
		};
		insertSymbol(currentCompilerState()->symbolTable, entry);
	}
	else if (array->type == INTERVAL_ARRAY) {
		// Interval arrays are not supported yet
		logError(_logger, "Interval arrays are not supported yet.");
		currentCompilerState()->succeed = false;
		return NULL;
	} else {
		logError(_logger, "The array '%s' is invalid.", identifier);
		currentCompilerState()->succeed = false;
		return NULL;
	}


	return sentence;
}

Sentence * LogSentenceSemanticAction(StringPartList * stringPartList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * logSentence = calloc(1, sizeof(Sentence));
	logSentence->logString = stringPartList;
	logSentence->type = LOG_SENTENCE;
	return logSentence;
}
StringPartList * appendStringPartList(StringPartList * stringPartList, StringPart * stringPart) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StringPartList * newStringPartList = calloc(1, sizeof(StringPartList));
	newStringPartList->stringPart = stringPart;
	newStringPartList->next = stringPartList;
	return newStringPartList;
}
StringPartList * createStringPartList(StringPart * stringPart) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StringPartList * stringPartList = calloc(1, sizeof(StringPartList));
	stringPartList->stringPart = stringPart;
	stringPartList->next = NULL;
	return stringPartList;
}
StringPart * createStringSegment(char * string) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StringPart * stringPart = calloc(1, sizeof(StringPart));
	stringPart->string = string;
	stringPart->type = STRING_SEGMENT;
	return stringPart;
}
StringPart * createStringInterpolation(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StringPart * stringPart = calloc(1, sizeof(StringPart));
	stringPart->identifier = identifier;
	stringPart->type = IDENTIFIER_SEGMENT;
	return stringPart;
}

Sentence * IfSentenceSemanticAction(BoolExpression * boolExpression, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->ifBoolExpression = boolExpression;
	sentence->ifBlock = block;
	sentence->type = IF_SENTENCE;
	return sentence;
	// TODO: Semantics needed
}

Sentence * ForSentenceSemanticAction(char * identifier, Array * array, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->forIdentifier = identifier;
	sentence->forArray = array;
	sentence->forBlock = block;
	sentence->type = FOR_SENTENCE;
	return sentence;
	// TODO: Semantics needed
}

Sentence * IfElseSentenceSemanticAction(BoolExpression * boolExpression, Block * leftBlock, Block * rightBlock) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->ifElseBoolExpression = boolExpression;
	sentence->leftIfElseBlock = leftBlock;
	sentence->rightIfElseBlock = rightBlock;
	sentence->type = IF_ELSE_SENTENCE;
	return sentence;
	// TODO: Semantics needed
}

Block * BlockSemanticAction(Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * block = calloc(1, sizeof(Block));
	block->sentences = sentences;
	return block;
}

Array * IntervalArraySemanticAction(Expression * leftExpression, Expression * rightExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Array * array = calloc(1, sizeof(Array));
	array->leftExpression = leftExpression;
	array->rightExpression = rightExpression;
	array->type = INTERVAL_ARRAY;
	return array;
}

Array * IdentifierArraySemanticAction(char * identifier) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Array * array = calloc(1, sizeof(Array));
	array->identifier = identifier;
	array->type = IDENTIFIER_ARRAY;

	// Semantic Analysis
	const SymbolEntry symbolEntry = getSymbolEntry(currentCompilerState()->symbolTable, identifier);
	if (symbolEntry.type != ARRAY_TYPE) {
		logError(_logger, "The identifier '%s' is not an array.", identifier);
		currentCompilerState()->succeed = false;
		return NULL;
	}
	return array;
}

Array * BasicArraySemanticAction(ExpressionList * expressionList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Array * array = calloc(1, sizeof(Array));
	array->expressionList = expressionList;
	array->type = BASIC_ARRAY;
	return array;
}

BoolExpression * BoolComparisonExpressionSemanticAction(Expression * leftExpression, Expression * rightExpression, BoolExpressionType type) {
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
	boolExpression->type = BOOL_FACTOR;
	return boolExpression;
}

BoolFactor * BoolExpressionFactorSemanticAction(BoolExpression * boolExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	BoolFactor * boolFactor = calloc(1, sizeof(BoolFactor));
	boolFactor->boolExpression = boolExpression;
	return boolFactor;
}

Expression * FunctionExpressionSemanticAction(char * identifier, ExpressionList * functionArguments) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Expression * expression = calloc(1, sizeof(Expression));
	expression->functionIdentifier = identifier;
	expression->functionArguments = functionArguments;
	expression->type = FUNCTION_EXPRESSION;
	return functionSemanticAnalyzerCheck(identifier, functionArguments)? expression : NULL;
}

Sentence * ImportSentenceSemanticAction(StringPartList * importPath) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->importPath = importPath;
	sentence->type = IMPORT_SENTENCE;
	return sentence;
}