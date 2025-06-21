#include "BisonActions.h"
#include "../../shared/CompilerState.h"
#include <math.h>
#include "BisonParser.h"
#include "../../backend/semantic-analysis/SymbolTable.h"

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

	// const SymbolEntry symbolEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, array->identifier, currentCompilerState()->scopesStack);
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
	const SymbolEntry symbolEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
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

	const SymbolEntry symbolEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
	if (symbolEntry.type != ARRAY_TYPE) {
		logError(_logger, "The identifier '%s' should be an array.", identifier);
		currentCompilerState()->succeed = false;
		return NULL;
	}
	if (typeOfExpression(indexExpression) != INTEGER_TYPE) {
		printf("Index expression type: %s\n", symbolTypeToString(typeOfExpression(indexExpression)));
		logError(_logger, "The index expression of the array '%s' should be an integer.", identifier);
		currentCompilerState()->succeed = false;
		return NULL;
	}
	if (typeOfExpression(expression) != symbolEntry.value.arrayData.elements[0].type) {
		printf("Expression type: %s\n", symbolTypeToString(typeOfExpression(expression)));
		printf("Array type: %s\n", symbolTypeToString(symbolEntry.value.arrayData.elements[0].type));
		logError(_logger, "The expression of the array '%s' should be of type '%s'.", identifier, symbolTypeToString(symbolEntry.value.arrayData.dataType));
		currentCompilerState()->succeed = false;
		return NULL;
	}
	
	return sentence;

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
	const SymbolEntry symbolEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
	if (symbolEntry.type != FUNCTION_TYPE) {
		logError(_logger, "The identifier '%s' is not a function.", identifier);
		currentCompilerState()->succeed = false;
		return false;
	}

	const SymbolType* fnParameterTypes = symbolEntry.value.functionData.parameterType;
	int parameterCount = symbolEntry.value.functionData.parameterCount;

	// Case where the function has no parameters (ExpressionList should be empty)
	if (parameterCount == 0) {
		if (functionArguments->expressions == NULL) {
			return true;
		}
		logError(_logger, "The function '%s' does not expect any parameters.", identifier);
		currentCompilerState()->succeed = false;
		return false;
	}

	// Case where the function has parameters
	const Expressions* expressionIndexer = functionArguments->expressions;
	for (int i=0; i < parameterCount; i++) {
		if (expressionIndexer == NULL) {
			logError(_logger, "The function '%s' is being called with not enough parameters.", identifier);
			return false;
		}
		SymbolType paramenterType = fnParameterTypes[i];
		SymbolType expressionType = typeOfExpression(expressionIndexer->expression);
		if (paramenterType != expressionType) {
			logError(_logger, "Invalid parameter in function '%s': Should be '%s' but found '%s'", identifier, symbolTypeToString(paramenterType), symbolTypeToString(expressionType));
			return false;
		}
		expressionIndexer = expressionIndexer->next;
	}
	// Additional operations if the function has infinite parameters
	if (parameterCount == -1) {
		while (expressionIndexer != NULL) {
			if (fnParameterTypes[0] != typeOfExpression(expressionIndexer->expression)) {
				logError(_logger, "Invalid parameter in function '%s': Should be '%s'.", identifier, symbolTypeToString(fnParameterTypes[0]));
				currentCompilerState()->succeed = false;
				return false;
			}
			expressionIndexer = expressionIndexer->next;
		}
	}
	else {
		if (expressionIndexer != NULL) {
			logError(_logger, "The function '%s' is being called with too many parameters.", identifier);
			currentCompilerState()->succeed = false;
			return false;
		}
	}
	return true;
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
		.scope = currentScope(currentCompilerState()->scopesStack),
	};
	
	const SymbolEntry currentEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
	if (expressionType == INVALID_TYPE) {
		return NULL;
	}

	if (currentEntry.type == NULL_TYPE) {
		insertSymbol(currentCompilerState()->symbolTable, symbolEntry);
	}
	else if (currentEntry.type != expressionType) {
		logError(_logger, "The identifier '%s' has type mismatch", identifier);
		return NULL;
	}

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
	const SymbolEntry currentEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
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

		BasicType* arrayElements = malloc(sizeof(SymbolEntry) * 256); // Assuming a maximum of 100 elements for simplicity

		Expressions* expressionsIndex = array->expressionList->expressions;
		SymbolType t = typeOfExpression(expressionsIndex->expression);

		// Check if all expressions in the array are of the same type
		int i = 0;
		for (; expressionsIndex != NULL; i++) {
			if (t != typeOfExpression(expressionsIndex->expression)) {
				logError(_logger, "The array '%s' has elements of different types.", identifier);
				currentCompilerState()->succeed = false;
				return NULL;
			}
			BasicType arrayElement = {
				.type = t, // Assuming the type of the array is the type of the elements
			};
			arrayElements[i] = arrayElement;
			expressionsIndex = expressionsIndex->next;
		}

		const SymbolEntry entry = {
			.identifier = identifier,
			.type = ARRAY_TYPE, // Assuming the type of the array is ARRAY_TYPE
			.value.arrayData = {
				.elements = arrayElements, // This will be filled later when the array is defined
				.dataType = BASIC_ARRAY, // Assuming the type of the array is the type of the identifier
				.size = i // Set the size of the array
			},
			.scope = currentScope(currentCompilerState()->scopesStack)
		};
		printf("Inserting array '%s' with %d elements of type %s\n", identifier, i, symbolTypeToString(t));
		insertSymbol(currentCompilerState()->symbolTable, entry);
	} else if (array->type == IDENTIFIER_ARRAY) {

		SymbolEntry otherArrayEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, array->identifier, currentCompilerState()->scopesStack);
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
			},
			.scope = currentScope(currentCompilerState()->scopesStack)
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

	StringPartList *current = stringPartList;
	while(current) {
		if(current->stringPart->type == IDENTIFIER_SEGMENT) {
			char *identifier = current->stringPart->identifier;
			SymbolEntry entry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
			if(entry.type == NULL_TYPE) {
				logError(_logger, "the identifier '%s' doesnt exist. used in string part", identifier);
				return NULL;
			}
		}
		current = current->next;
	} 

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

	if(!boolExpressionIsValid(boolExpression)) {
		logError(_logger, "The boolean expression is not valid.");
		currentCompilerState()->succeed = false;
		return NULL;
	}
	return sentence;
}

int InsertForLoopIterator(char * identifier, Array * array) {
	SymbolType elementType = INVALID_TYPE;
	switch (array->type)
	{
	case IDENTIFIER_ARRAY:
		SymbolEntry arrayEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, array->identifier, currentCompilerState()->scopesStack);
		if (arrayEntry.type == ARRAY_TYPE) {
			elementType = arrayEntry.value.arrayData.elements[0].type;
		}
		break;
	case INTERVAL_ARRAY:
		SymbolType leftType = typeOfExpression(array->leftExpression);
		SymbolType rightType = typeOfExpression(array->rightExpression);
		if(leftType == INTEGER_TYPE && rightType == INTEGER_TYPE) {
			elementType = INTEGER_TYPE;
		}
		break;
	case BASIC_ARRAY:
		elementType = typeOfExpression(array->expressionList->expressions->expression);
		
		// Check if all expressions in the array are of the same type (TODO: this code is repeated and could be refactored)
		Expressions* expressionsIndex = array->expressionList->expressions;

		int i = 0;
		for (; expressionsIndex != NULL; i++) {
			if (elementType != typeOfExpression(expressionsIndex->expression)) {
				elementType = INVALID_TYPE;
				break;
			}
			expressionsIndex = expressionsIndex->next;
		}
	default:
		break;
	}
	if(elementType == INVALID_TYPE) {
		return false;
	}

	insertSymbol(currentCompilerState()->symbolTable, (SymbolEntry) {
		.scope = getNextScope(currentCompilerState()->scopesStack), 
		.identifier = identifier,
		.type = elementType, // Assuming the type of the array is ARRAY_TYPE
	});

	return true;
}

Sentence * ForSentenceSemanticAction(char * identifier, Array * array, Block * block) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Sentence * sentence = calloc(1, sizeof(Sentence));
	sentence->forIdentifier = identifier;
	sentence->forArray = array;
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

	if(!boolExpressionIsValid(boolExpression)) {
		logError(_logger, "The boolean expression is not valid.");
		currentCompilerState()->succeed = false;
		return NULL;
	}

	return sentence;
}

Block * BlockSemanticAction(Sentences * sentences) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Block * block = calloc(1, sizeof(Block));
	block->sentences = sentences;

	// End Scope
	popScopesStack(currentCompilerState()->scopesStack);

	return block;
}

void StartScope() {
	addNewScope(currentCompilerState()->scopesStack);
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
	const SymbolEntry symbolEntry = getSymbolEntryWithScope(currentCompilerState()->symbolTable, identifier, currentCompilerState()->scopesStack);
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