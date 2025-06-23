#ifndef GENERATOR_HEADER
#define GENERATOR_HEADER

#include "../../frontend/syntactic-analysis/AbstractSyntaxTree.h"
#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../backend/semantic-analysis/SymbolTable.h"
#include <stdarg.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/** Initialize module's internal state. */
void initializeGeneratorModule();

/** Shutdown module's internal state. */
void shutdownGeneratorModule();

/**
 * Generates the final SVG output using the current compiler state.
 */
void generate(CompilerState * compilerState);

/**
 * Generates the final SVG output to a specific file.
 */
void generateToFile(CompilerState * compilerState, const char * outputPath);

/**
 * Sets the SVG canvas dimensions
 */
void setSVGDimensions(int width, int height);

/**
 * SVG Generation Context to maintain drawing state
 */
typedef struct {
    int fillColor;
    int strokeColor;
    int strokeWidth;
    int layer;
    int width;
    int height;
    int centerX;
    int centerY;
} SVGContext;

/**
 * Generates a polygon from a list of vector points
 */
static void _generatePolygon(FILE* outputFile, ExpressionList * arguments);

#endif
