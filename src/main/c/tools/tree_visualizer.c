#include "../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../frontend/syntactic-analysis/BisonActions.h"
#include "../frontend/syntactic-analysis/SyntacticAnalyzer.h"
#include "../frontend/lexical-analysis/FlexActions.h"
#include "../shared/CompilerState.h"
#include "../shared/Logger.h"
#include "../shared/Type.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

// Forward declarations
static void print_factor(Factor* factor, int level, bool is_last);
static void print_bool_factor(BoolFactor* factor, int level, bool is_last);
static void print_sentences(Sentences* sentences, int level, bool is_last);
static void print_expression(Expression* expr, int level, bool is_last);
static void print_bool_expression(BoolExpression* expr, int level, bool is_last);
static void print_block(Block* block, int level, bool is_last);
static void print_interval(Interval* interval, int level, bool is_last);
static void print_sentence(Sentence* sentence, int level, bool is_last);

// Function to print indentation with tree lines
static void print_indent(int level, bool is_last) {
    for (int i = 0; i < level; i++) {
        if (i == level - 1) {
            printf(is_last ? "└── " : "├── ");
        } else {
            printf("│   ");
        }
    }
}

// Function to print expression type
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
        default: return "UNKNOWN";
    }
}

// Function to print factor type
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

// Function to print sentence type
static const char* sentence_type_to_string(SentenceType type) {
    switch (type) {
        case IF_SENTENCE: return "IF";
        case IF_ELSE_SENTENCE: return "IF_ELSE";
        case ASSIGN_SENTENCE: return "ASSIGN";
        case FOR_SENTENCE: return "FOR";
        case FUNCTION_SENTENCE: return "FUNCTION";
        case ASSIGN_ARRAY_SENTENCE: return "ASSIGN_ARRAY";
        case LOG_SENTENCE: return "LOG";
        default: return "UNKNOWN";
    }
}

// Function to print bool expression type
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

// Function to print factor
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

// Function to print expression
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
    }
}

// Function to print bool factor
static void print_bool_factor(BoolFactor* factor, int level, bool is_last) {
    if (!factor) return;
    print_bool_expression(factor->boolExpression, level, is_last);
}

// Function to print bool expression
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

// Function to print sentences
static void print_sentences(Sentences* sentences, int level, bool is_last) {
    if (!sentences) return;
    
    print_sentences(sentences->sentences, level, false);
    print_sentence(sentences->sentence, level, is_last);
}

// Function to print block
static void print_block(Block* block, int level, bool is_last) {
    if (!block) return;
    print_sentences(block->sentences, level, is_last);
}

// Function to print interval
static void print_interval(Interval* interval, int level, bool is_last) {
    if (!interval) return;
    
    print_indent(level, is_last);
    printf("Interval\n");
    
    switch (interval->type) {
        case RANGE_INTERVAL:
            print_expression(interval->leftExpression, level + 1, false);
            print_expression(interval->rightExpression, level + 1, true);
            break;
        case IDENTIFIER_INTERVAL:
            print_indent(level + 1, true);
            printf("Identifier: %s\n", interval->identifier);
            break;
    }
}

// Function to print sentence
static void print_sentence(Sentence* sentence, int level, bool is_last) {
    if (!sentence) return;
    
    print_indent(level, is_last);
    printf("Sentence (%s)\n", sentence_type_to_string(sentence->type));
    
    switch (sentence->type) {
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
            printf("Identifier: %s\n", sentence->assignIdentifier);
            print_expression(sentence->assignExpression, level + 1, true);
            break;
        case FOR_SENTENCE:
            print_indent(level + 1, false);
            printf("Identifier: %s\n", sentence->forIdentifier);
            print_interval(sentence->forInterval, level + 1, false);
            print_block(sentence->forBlock, level + 1, true);
            break;
        case FUNCTION_SENTENCE:
            print_indent(level + 1, true);
            printf("Function: %s\n", sentence->functionIdentifier);
            // TODO: Print function arguments
            break;
        case ASSIGN_ARRAY_SENTENCE:
            print_indent(level + 1, true);
            printf("Array: %s\n", sentence->assignArrayIdentifier);
            // TODO: Print array expressions
            break;
        case LOG_SENTENCE:
            print_indent(level + 1, true);
            printf("Log\n");
            // TODO: Print log string parts
            break;
    }
}

// Function to print program
static void print_program(Program* program) {
    if (!program) return;
    print_sentences(program->sentences, 0, true);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s <test_file>\n", argv[0]);
        return 1;
    }

    // Initialize modules
    initializeFlexActionsModule();
    initializeBisonActionsModule();
    initializeSyntacticAnalyzerModule();
    initializeAbstractSyntaxTreeModule();

    // Open and read the test file
    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Create a pipe for stdin redirection
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        printf("Error: Could not create pipe\n");
        fclose(file);
        return 1;
    }

    // Read the file content
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
    close(pipefd[1]); // Close write end

    // Redirect stdin to read from the pipe
    if (dup2(pipefd[0], STDIN_FILENO) == -1) {
        printf("Error: Could not redirect stdin\n");
        close(pipefd[0]);
        fclose(file);
        return 1;
    }
    close(pipefd[0]); // Close original read end

    // Create compiler state
    CompilerState compilerState = {
        .abstractSyntaxtTree = NULL,
        .succeed = false,
        .value = 0
    };

    // Parse the file
    const SyntacticAnalysisStatus status = parse(&compilerState);
    
    // Close the file
    fclose(file);

    if (status == ACCEPT) {
        printf("\nTest file: %s\n", argv[1]);
        print_program(compilerState.abstractSyntaxtTree);
    } else {
        printf("\nError: Failed to parse %s\n", argv[1]);
    }

    // Cleanup
    releaseProgram(compilerState.abstractSyntaxtTree);
    shutdownAbstractSyntaxTreeModule();
    shutdownSyntacticAnalyzerModule();
    shutdownBisonActionsModule();
    shutdownFlexActionsModule();

    return status == ACCEPT ? 0 : 1;
} 