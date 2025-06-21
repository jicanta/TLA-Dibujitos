#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../frontend/syntactic-analysis/BisonActions.h"
#include "../frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "../frontend/lexical-analysis/FlexActions.h"
#include "../shared/CompilerState.h"
#include "../shared/Logger.h"
#include "../backend/semantic-analysis/SymbolTable.h"
#include "../shared/Type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

static void print_factor(Factor* factor, int level, bool is_last);
static void print_bool_factor(BoolFactor* factor, int level, bool is_last);
static void print_sentences(Sentences* sentences, int level, bool is_last);
static void print_expression(Expression* expr, int level, bool is_last);
static void print_bool_expression(BoolExpression* expr, int level, bool is_last);
static void print_block(Block* block, int level, bool is_last);
static void print_array(Array* array, int level, bool is_last);
static void print_sentence(Sentence* sentence, int level, bool is_last);
static void print_expression_list(ExpressionList* expr_list, int level, bool is_last);
static void print_expressions(Expressions* expressions, int level, bool is_last);
static void print_string_part_list(StringPartList* string_part_list, int level, bool is_last);
static void print_string_part(StringPart* string_part, int level, bool is_last);

static void print_indent(int level, bool is_last) {
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf(is_last ? "└── " : "├── ");
        } else {
            printf("│   ");
        }
    }
}

static const char* expression_type_to_string(ExpressionType type) {
    switch (type) {
        case ADDITION: return "ADDITION";
        case SUBTRACTION: return "SUBTRACTION";
        case MULTIPLICATION: return "MULTIPLICATION";
        case DIVISION: return "DIVISION";
        case FACTOR: return "FACTOR";
        case MODULUS: return "MODULUS";
        case GET_X: return "GET_X";
        case GET_Y: return "GET_Y";
        case ARRAY_ACCESS: return "ARRAY_ACCESS";
        case FUNCTION_EXPRESSION: return "FUNCTION_EXPRESSION";
        default: return "UNKNOWN";
    }
}

static const char* factor_type_to_string(FactorType type) {
    switch (type) {
        case IDENTIFIER_FACTOR: return "IDENTIFIER";
        case INTEGER_FACTOR: return "INTEGER";
        case VECTOR_FACTOR: return "VECTOR";
        case DECIMAL_FACTOR: return "DECIMAL";
        case PARENTHESIS_FACTOR: return "PARENTHESIS";
        default: return "UNKNOWN";
    }
}

static const char* sentence_type_to_string(SentenceType type) {
    switch (type) {
        case IF_SENTENCE: return "IF";
        case IF_ELSE_SENTENCE: return "IF_ELSE";
        case ASSIGN_SENTENCE: return "ASSIGN";
        case FOR_SENTENCE: return "FOR";
        case FUNCTION_SENTENCE: return "FUNCTION";
        case ASSIGN_ARRAY_SENTENCE: return "ASSIGN_ARRAY";
        case ASSIGN_ARRAY_ELEMENT_SENTENCE: return "ASSIGN_ARRAY_ELEMENT";
        case LOG_SENTENCE: return "LOG";
        case IMPORT_SENTENCE: return "IMPORT";
        default: return "UNKNOWN";
    }
}

static const char* bool_expression_type_to_string(BoolExpressionType type) {
    switch (type) {
        case GREATER_OR_EQUAL: return ">=";
        case LESS_OR_EQUAL: return "<=";
        case GREATER_THAN: return ">";
        case LESS_THAN: return "<";
        case EQUAL_TO: return "==";
        case NOT_EQUAL: return "!=";
        case AND_TYPE: return "AND";
        case OR_TYPE: return "OR";
        case NOT_TYPE: return "NOT";
        case BOOL_FACTOR: return "BOOL_FACTOR";
        default: return "UNKNOWN";
    }
}

static void print_factor(Factor* factor, int level, bool is_last) {
    if (!factor) return;
    
    print_indent(level, is_last);
    printf("Factor (%s)\n", factor_type_to_string(factor->type));
    
    switch (factor->type) {
        case IDENTIFIER_FACTOR:
            print_indent(level + 1, true);
            printf("Identifier: %s\n", factor->identifier);
            break;
        case INTEGER_FACTOR:
            print_indent(level + 1, true);
            printf("Integer: %d\n", factor->integerExpression);
            break;
        case DECIMAL_FACTOR:
            print_indent(level + 1, true);
            printf("Decimal: %f\n", factor->floatExpression);
            break;
        case PARENTHESIS_FACTOR:
            print_expression(factor->expression, level + 1, true);
            break;
        case VECTOR_FACTOR:
            print_indent(level + 1, false);
            printf("Vector:\n");
            print_expression(factor->vector->x, level + 2, false);
            print_expression(factor->vector->y, level + 2, true);
            break;
    }
}

static void print_expression(Expression* expr, int level, bool is_last) {
    if (!expr) return;
    
    print_indent(level, is_last);
    printf("Expression (%s)\n", expression_type_to_string(expr->type));
    
    switch (expr->type) {
        case ADDITION:
        case SUBTRACTION:
        case MULTIPLICATION:
        case DIVISION:
        case MODULUS:
            print_expression(expr->leftExpression, level + 1, false);
            print_expression(expr->rightExpression, level + 1, true);
            break;
        case GET_X:
        case GET_Y:
            print_expression(expr->expression, level + 1, true);
            break;
        case FACTOR:
            print_factor(expr->factor, level + 1, true);
            break;
        case ARRAY_ACCESS:
            print_array(expr->array, level + 1, true);
            print_expression(expr->indexExpression, level + 1, true);
            break;
        case FUNCTION_EXPRESSION:
            print_indent(level + 1, true);
            printf("Function: %s\n", expr->functionIdentifier);
            if (expr->functionArguments) {
                print_expression_list(expr->functionArguments, level + 1, true);
            }
            break;
    }
}

static void print_bool_factor(BoolFactor* factor, int level, bool is_last) {
    if (!factor) return;
    print_bool_expression(factor->boolExpression, level, is_last);
}

static void print_bool_expression(BoolExpression* expr, int level, bool is_last) {
    if (!expr) return;
    
    print_indent(level, is_last);
    printf("BoolExpression (%s)\n", bool_expression_type_to_string(expr->type));
    
    switch (expr->type) {
        case GREATER_OR_EQUAL:
        case LESS_OR_EQUAL:
        case GREATER_THAN:
        case LESS_THAN:
        case EQUAL_TO:
        case NOT_EQUAL:
            print_expression(expr->leftExpression, level + 1, false);
            print_expression(expr->rightExpression, level + 1, true);
            break;
        case AND_TYPE:
        case OR_TYPE:
            print_bool_expression(expr->leftBoolExpression, level + 1, false);
            print_bool_expression(expr->rightBoolExpression, level + 1, true);
            break;
        case NOT_TYPE:
            print_bool_expression(expr->boolExpression, level + 1, true);
            break;
        case BOOL_FACTOR:
            print_bool_factor(expr->boolFactor, level + 1, true);
            break;
    }
}

static void print_sentences(Sentences* sentences, int level, bool is_last) {
    if (!sentences) return;
    
    print_sentences(sentences->sentences, level, false);
    print_sentence(sentences->sentence, level, is_last);
}

static void print_block(Block* block, int level, bool is_last) {
    if (!block) return;
    print_sentences(block->sentences, level, is_last);
}

static void print_array(Array* array, int level, bool is_last) {
    if (!array) return;
    
    print_indent(level, is_last);
    printf("Array\n");
    
    switch (array->type) {
        case INTERVAL_ARRAY:
            print_expression(array->leftExpression, level + 1, false);
            print_expression(array->rightExpression, level + 1, true);
            break;
        case IDENTIFIER_ARRAY:
            print_indent(level + 1, true);
            printf("Identifier: %s\n", array->identifier);
            break;
        case BASIC_ARRAY:
            if (array->expressionList) {
                print_expression_list(array->expressionList, level + 1, true);
            }
            break;
    }
}

static void print_sentence(Sentence* sentence, int level, bool is_last) {
    if (!sentence) return;
    
    print_indent(level, is_last);
    printf("Sentence (%s)\n", sentence_type_to_string(sentence->type));
    
    switch (sentence->type) {
        case FOR_SENTENCE:
            print_indent(level + 1, false);
            printf("Identifier: %s\n", sentence->forIdentifier);
            print_array(sentence->forArray, level + 1, false);
            print_block(sentence->forBlock, level + 1, true);
            break;
        case IF_SENTENCE:
            print_bool_expression(sentence->ifBoolExpression, level + 1, false);
            print_block(sentence->ifBlock, level + 1, true);
            break;
        case IF_ELSE_SENTENCE:
            print_bool_expression(sentence->ifElseBoolExpression, level + 1, false);
            print_block(sentence->leftIfElseBlock, level + 1, false);
            print_block(sentence->rightIfElseBlock, level + 1, true);
            break;
        case ASSIGN_SENTENCE:
            print_indent(level + 1, false);
            printf("Identifier: %s INFERRED_TYPE", sentence->assignIdentifier);
            printSymbolEntry(getSymbolEntry(currentCompilerState()->symbolTable, sentence->assignIdentifier));
            puts("");
            print_expression(sentence->assignExpression, level + 1, true);
            break;
        case FUNCTION_SENTENCE:
            print_indent(level + 1, true);
            printf("Function: %s\n", sentence->functionIdentifier);
            if (sentence->functionArguments) {
                print_expression_list(sentence->functionArguments, level + 1, true);
            }
            break;
        case ASSIGN_ARRAY_SENTENCE:
            print_indent(level + 1, true);
            printf("Array: %s\n", sentence->assignArrayIdentifier);
            if (sentence->assignArray) {
                print_array(sentence->assignArray, level + 1, true);
            }
            break;
        case ASSIGN_ARRAY_ELEMENT_SENTENCE:
            print_indent(level + 1, true);
            printf("Array Element: %s\n", sentence->assignArrayElemIdentifier);
            print_expression(sentence->assignArrayIndexExpression, level + 1, false);
            print_expression(sentence->assignArrayElementExpression, level + 1, true);
            break;
        case LOG_SENTENCE:
            print_indent(level + 1, true);
            printf("Log\n");
            if (sentence->logString) {
                print_string_part_list(sentence->logString, level + 1, true);
            }
            break;
        case IMPORT_SENTENCE:
            print_indent(level + 1, true);
            printf("Import\n");
            if (sentence->importPath) {
                print_string_part_list(sentence->importPath, level + 1, true);
            }
            break;
    }
}

static void print_program(Program* program) {
    if (!program) return;
    print_sentences(program->sentences, 0, true);
}

static void print_expression_list(ExpressionList* expr_list, int level, bool is_last) {
    if (!expr_list) return;
    
    print_indent(level, is_last);
    printf("ExpressionList\n");
    
    if (expr_list->expressions) {
        print_expressions(expr_list->expressions, level + 1, true);
    }
}

static void print_expressions(Expressions* expressions, int level, bool is_last) {
    if (!expressions) return;
    
    print_expression(expressions->expression, level, is_last);
    print_expressions(expressions->next, level, false);
}

static void print_string_part_list(StringPartList* string_part_list, int level, bool is_last) {
    if (!string_part_list) return;
    
    print_string_part_list(string_part_list->next, level, false);
    print_string_part(string_part_list->stringPart, level, is_last);
}

static void print_string_part(StringPart* string_part, int level, bool is_last) {
    if (!string_part) return;
    
    print_indent(level, is_last);
    printf("StringPart (%s)\n", string_part->type == STRING_SEGMENT ? "STRING" : "IDENTIFIER");
    
    if (string_part->type == STRING_SEGMENT) {
        print_indent(level + 1, true);
        printf("String: %s\n", string_part->string);
    } else {
        print_indent(level + 1, true);
        printf("Identifier: %s\n", string_part->identifier);
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <test_file>\n", argv[0]);
        return 1;
    }

    initializeFlexActionsModule();
    initializeBisonActionsModule();
    initializeSyntacticAnalyzerModule();
    initializeAbstractSyntaxTreeModule();

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error: Could not create pipe\n");
        fclose(file);
        return 1;
    }

    char buffer[4096];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (write(pipefd[1], buffer, bytes_read) != bytes_read) {
            printf("Error: Could not write to pipe\n");
            close(pipefd[0]);
            close(pipefd[1]);
            fclose(file);
            return 1;
        }
    }
    close(pipefd[1]);

    if (dup2(pipefd[0], STDIN_FILENO) == -1) {
        printf("Error: Could not redirect stdin\n");
        close(pipefd[0]);
        fclose(file);
        return 1;
    }
    close(pipefd[0]);

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
    // TODO: Initialize scopes stack properly
    setDefaultFunctions(compilerState.symbolTable, initialScope);
    const SyntacticAnalysisStatus status = parse(&compilerState);
    
    fclose(file);

    if (status == ACCEPT) {
        printf("\nTest file: %s\n", argv[1]);
        printSymbolTable(compilerState.symbolTable);
        print_program(compilerState.abstractSyntaxtTree);
    } else {
        printf("\nError: Failed to parse %s\n", argv[1]);
    }
    freeSymbolTable(compilerState.symbolTable);
    releaseProgram(compilerState.abstractSyntaxtTree);
    shutdownAbstractSyntaxTreeModule();
    shutdownSyntacticAnalyzerModule();
    shutdownBisonActionsModule();
    shutdownFlexActionsModule();

    return status == ACCEPT ? 0 : 1;
} 