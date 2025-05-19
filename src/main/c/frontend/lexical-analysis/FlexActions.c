#include "FlexActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;
static boolean _logIgnoredLexemes = true;

void initializeFlexActionsModule() {
	_logIgnoredLexemes = getBooleanOrDefault("LOG_IGNORED_LEXEMES", _logIgnoredLexemes);
	_logger = createLogger("FlexActions");
}

void shutdownFlexActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/* PRIVATE FUNCTIONS */

static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext);

/**
 * Logs a lexical-analyzer context in DEBUGGING level.
 */
static void _logLexicalAnalyzerContext(const char * functionName, LexicalAnalyzerContext * lexicalAnalyzerContext) {
	char * escapedLexeme = escape(lexicalAnalyzerContext->lexeme);
	logDebugging(_logger, "%s: %s (context = %d, length = %d, line = %d)",
		functionName,
		escapedLexeme,
		lexicalAnalyzerContext->currentContext,
		lexicalAnalyzerContext->length,
		lexicalAnalyzerContext->line);
	free(escapedLexeme);
}

static void _logDebug(const char * functionName, const char * message) {
	logDebugging(_logger, "%s: %s", functionName, message);
}

/* PUBLIC FUNCTIONS */

void BeginMultilineCommentLexemeAction() {
	if (_logIgnoredLexemes) {
		_logDebug(__FUNCTION__, "/*");
	}
}

void EndMultilineCommentLexemeAction() {
	if (_logIgnoredLexemes) {
		_logDebug(__FUNCTION__, "*/");
	}
}

void IgnoredLexemeAction(char * text) {
	if (_logIgnoredLexemes) {
		_logDebug(__FUNCTION__, text);
	}
}

Token ArithmeticOperatorLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);
	yylval.token = token;
	return token;
}

Token IntegerLexemeAction(char * text) {
	_logDebug(__FUNCTION__, text);
	yylval.integer = atoi(text);

	return INTEGER;
}

Token DecimalLexemeAction(char *text) {
	_logDebug(__FUNCTION__, text);
    yylval.decimal = strtof(text, NULL);

    return DECIMAL;
}

Token ParenthesisLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);	
	yylval.token = token;
	return token;
}

Token UnknownLexemeAction(char *text) {
	_logDebug(__FUNCTION__, text);

	return UNKNOWN;
}

Token BinaryOperatorLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);
	yylval.token = token;
	return token;
}

Token UnaryOperatorLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);
	yylval.token = token;

	return token;
}

Token AssignmentLexemeAction() {
	_logDebug(__FUNCTION__, "assignment =");	

	return ASSIGN;
}

Token CommaLexemeAction() {
	_logDebug(__FUNCTION__, "comma ,");

	return COMMA;
}

Token SemicolonLexemeAction() {
	_logDebug(__FUNCTION__, "semicolon ;");

	return SEMICOLON;
}

Token BeginInterpolatedStringLexemeAction() {
    _logDebug(__FUNCTION__, "\"begin string\"");       
	
	return BEGIN_STRING;
}

Token EndInterpolatedStringLexemeAction() {
	_logDebug(__FUNCTION__, "\"end string\"");

	return END_STRING;
}

Token LogLexemeAction() {
	_logDebug(__FUNCTION__, "log");

	return LOG;
}

Token DotLexemeAction() {
	_logDebug(__FUNCTION__, "dot .");
	return DOT;
}

Token ColonLexemeAction() {
	_logDebug(__FUNCTION__, "colon :");
	return COLON;
}

Token ImportLexemeAction() {
	_logDebug(__FUNCTION__, "import");
	return IMPORT;
}

Token IfLexemeAction() {
	_logDebug(__FUNCTION__, "if");
	return IF;
}

Token ElseLexemeAction() {
	_logDebug(__FUNCTION__, "else");
	return ELSE;
}

Token InLexemeAction() {
	_logDebug(__FUNCTION__, "in");
	return IN;
}

Token ForLexemeAction() {
	_logDebug(__FUNCTION__, "for");
	return FOR;
}

Token BracesLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);
	yylval.token = token;

	return token;
}

Token BracketsLexemeAction(Token token, char *text) {
	_logDebug(__FUNCTION__, text);
	yylval.token = token;

	return token;
}

Token FloatKeywordLexemeAction() {
	_logDebug(__FUNCTION__, "float keyword");
	return FLOAT_KEYWORD;
}

Token IntKeywordLexemeAction() {
	_logDebug(__FUNCTION__, "int keyword");
	return INT_KEYWORD;
}

Token VectorKeywordLexemeAction() {
	_logDebug(__FUNCTION__, "vector keyword");
	return VECTOR_KEYWORD;
}

Token XParamLexemeAction() {
	_logDebug(__FUNCTION__, "x param'");
	return X_PARAM;
}

Token YParamLexemeAction() {
	_logDebug(__FUNCTION__, "y param'");
	return Y_PARAM;
}

Token StringLexemeAction(char * text_pointer, int text_length) {
	_logDebug(__FUNCTION__, text_pointer);

	yylval.string = NULL; // Initialize to NULL to avoid dangling pointer
	// Strip the surrounding quotes (assuming they are present)
	if (text_length >= 2 && text_pointer[0] == '"' && text_pointer[text_length - 1] == '"') {
		char * unquoted = malloc(text_length - 1); // yyleng - 2 for quotes + 1 for '\0'
		if (!unquoted) {
			exit(EXIT_FAILURE); // Handle malloc failure gracefully if needed
		}
		strncpy(unquoted, text_pointer + 1, text_length - 2);
		unquoted[text_length - 2] = '\0';
		yylval.string = unquoted;
	}

	return STRING;
}

Token IdentifierLexemeAction(char * text_pointer, int text_length) {
	_logDebug(__FUNCTION__, text_pointer);
	char * identifier_name = malloc(text_length + 1); // yyleng + 1 for '\0'

	if (!identifier_name) {
		exit(EXIT_FAILURE); // Handle malloc failure gracefully if needed
	}
	strncpy(identifier_name, text_pointer, text_length);

	identifier_name[text_length] = '\0';

	yylval.string = identifier_name;

	return IDENTIFIER;
}
