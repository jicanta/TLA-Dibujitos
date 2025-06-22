#include "backend/code-generation/Generator.h"
#include "frontend/lexical-analysis/FlexActions.h"
#include "frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "frontend/syntactic-analysis/BisonActions.h"
#include "frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "shared/CompilerState.h"
#include "shared/Environment.h"
#include "shared/Logger.h"
#include "shared/String.h"
#include "backend/semantic-analysis/SymbolTable.h"
#include "backend/semantic-analysis/Scopes.h"
#include <string.h>

/**
 * The main entry-point of the entire application. If you use "strtok" to
 * parse anything inside this project instead of using Flex and Bison, I will
 * find you, and I will kill you (Bryan Mills; "Taken", 2008).
 */
const int main(const int count, const char ** arguments) {
	Logger * logger = createLogger("EntryPoint");

	// Check for output file argument
	const char* outputFile = NULL;
	for (int i = 1; i < count; i++) {
		if (strcmp(arguments[i], "-o") == 0 && i + 1 < count) {
			outputFile = arguments[i + 1];
			logDebugging(logger, "Output file specified: %s", outputFile);
			break;
		}
	}

	initializeFlexActionsModule();
	initializeBisonActionsModule();
	initializeSyntacticAnalyzerModule();
	initializeAbstractSyntaxTreeModule();
	initializeGeneratorModule();

	// Logs the arguments of the application.
	for (int k = 0; k < count; ++k) {
		logDebugging(logger, "Argument %d: \"%s\"", k, arguments[k]);
	}

	// Begin compilation process.
	CompilerState compilerState = {
		.abstractSyntaxtTree = NULL,
		.succeed = false,
		.symbolTable = createSymbolTable(),
		.value = 0,
		.scopeLevel = 1, // TODO: remove
		.scopesStack = initializeScopesStack()
	};

	addNewScope(compilerState.scopesStack);
	const int initialScope = currentScope(compilerState.scopesStack);
	
	setDefaultFunctions(compilerState.symbolTable, initialScope);
	
	const SyntacticAnalysisStatus syntacticAnalysisStatus = parse(&compilerState);
	CompilationStatus compilationStatus = SUCCEED;
	Program * program = compilerState.abstractSyntaxtTree;
	if (syntacticAnalysisStatus == ACCEPT) {

		printSymbolTable(compilerState.symbolTable);
		compilerState.succeed = true;
		// ----------------------------------------------------------------------------------------
		// Beginning of the Backend...-------------------------------------------------------------
		logDebugging(logger, "Generating SVG output...");
		if (outputFile) {
			logDebugging(logger, "Using output file: %s", outputFile);
			generateToFile(&compilerState, outputFile);
		} else {
			logDebugging(logger, "No output file specified, using stdout");
			generate(&compilerState);
		}
		if (!compilerState.succeed) {
			logError(logger, "The code-generation rejects the input program.");
			compilationStatus = FAILED;
		}
		// ...end of the Backend. -----------------------------------------------------------------
		// ----------------------------------------------------------------------------------------
	}
	else {
		logError(logger, "The syntactic-analysis phase rejects the input program.");
		compilationStatus = FAILED;
	}
	logDebugging(logger, "Releasing AST resources...");
	releaseProgram(program);
	logDebugging(logger, "Releasing modules resources...");

	freeSymbolTable(compilerState.symbolTable);
	freeScopesStack(compilerState.scopesStack);
	shutdownGeneratorModule();
	shutdownAbstractSyntaxTreeModule();
	shutdownSyntacticAnalyzerModule();
	shutdownBisonActionsModule();
	shutdownFlexActionsModule();
	logDebugging(logger, "Compilation is done.");
	destroyLogger(logger);
	return compilationStatus;
}
