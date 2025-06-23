[![✗](https://img.shields.io/badge/Release-v1.0.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# Dibujitos - Domain Specific Language for SVG Graphics

## Project Description

Dibujitos is a domain-specific language (DSL) designed for creating 2D drawings and vector graphics using geometric figures and mathematical expressions. The language provides a simple yet expressive syntax that allows users to programmatically define designs, patterns, and complex geometric figures.

Key features include:
- **Vector operations**: Full support for 2D vector arithmetic and manipulation
- **Mathematical expressions**: Built-in trigonometric, logarithmic, and arithmetic functions
- **Geometric primitives**: Circles, lines, curves, and polygons
- **Arrays and iteration**: Support for arrays and for-each loops
- **String interpolation**: Dynamic text generation with variable substitution
- **Type inference**: Static typing with automatic type inference
- **SVG output**: High-quality scalable vector graphics generation

The output is generated in SVG format, ensuring compatibility with web browsers, graphic editors, and other SVG-supporting tools. This DSL bridges the gap between programming and visual design, making it valuable for:
- Educational contexts (teaching programming through visual feedback)
- Artistic expression (procedural art and generative design)
- Technical visualization (mathematical plots and diagrams)
- Rapid prototyping of vector graphics

### Example Code

```javascript
// Draw a colorful spiral
fill(RED);
for i in [1:10] {
    angle = i * 0.5;
    radius = i * 5.0;
    x = cos(angle) * radius;
    y = sin(angle) * radius;
    circle((x, y), i * 2.0);
}
```

### Tree Visualizer

The project includes a comprehensive tree visualizer tool that provides detailed inspection of the compilation process:

**Features:**
- **AST Visualization**: Hierarchical display of the Abstract Syntax Tree
- **Symbol Table Inspection**: Shows variable types, scopes, and values
- **Type Information**: Displays inferred types for all variables and expressions
- **Debug Information**: Helps identify parsing and semantic analysis issues
- **Test Suite Integration**: Automatically runs on all test cases

**Usage:**
```bash
./tree_visualizer <test_file.dib>
```

The visualizer outputs:
1. Complete symbol table with types and values
2. Hierarchical AST structure with indentation
3. Type annotations for variables and expressions
4. Detailed breakdown of complex expressions

This tool is invaluable for:
- Understanding the structure of parsed code
- Debugging syntax and semantic analysis issues
- Validating compiler correctness
- Educational purposes (showing how code is parsed)

## Language Features

### Data Types
- **Integers**: `42`, `0xFF` (hexadecimal)
- **Floats**: `3.14`, `-2.5`
- **Vectors**: `(x, y)` coordinate pairs
- **Arrays**: `[1, 2, 3]` or `[1:5]` (range notation)
- **Strings**: `"Hello {variable}"` (with interpolation)

### Built-in Functions
- **Math**: `sqrt()`, `sin()`, `cos()`, `tan()`, `abs()`, `log()`, `pow()`, `round()`
- **Graphics**: `circle()`, `line()`, `curve()`, `polygon()`, `fill()`, `stroke()`, `layer()`
- **I/O**: `log()` for console output

### Control Structures
- **Conditionals**: `if (condition) { ... } else { ... }`
- **Loops**: `for item in array { ... }`
- **Variable assignment**: `variable = expression;`
- **Array operations**: `array[index] = value;`

## Team Members

| Name | Last Name | ID | Email |
|------|-----------|-----|-------|
| Juan Ignacio | Cantarella | 64509 | jcantarella@itba.edu.ar |
| Máximo Daniel | Carranza | 64538 | mcarranza@itba.edu.ar |
| Alex | Köhler | 64320 | akohler@itba.edu.ar |
| Javier | Liu | 64332 | jaliu@itba.edu.ar |

## Environment Configuration

Set the following environment variables to control the application behavior:

|Name|Default|Description|
|-|:-:|-|
|`LOG_IGNORED_LEXEMES`|`true`|When `true`, logs all ignored lexemes found with Flex at DEBUGGING level. Set to `false` to reduce console output.|
|`LOGGING_LEVEL`|`INFORMATION`|Minimum logging level. Available levels: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR`, `CRITICAL`.|
|`SHOW_AST`|`false`|When `true`, enables visualization of the Abstract Syntax Tree in console output.|

## Installation

The project supports multiple platforms and installation methods:

* [Docker Installation](doc/readme/Docker.md) - Recommended for cross-platform development
* [Microsoft Windows](doc/readme/Windows.md) - Native Windows setup
* [Ubuntu/Linux](doc/readme/Ubuntu.md) - Linux development environment

## Getting Started

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd TLA-Dibujitos
   ```

2. **Build the project**
   ```bash
   make all
   ```

3. **Run a test**
   ```bash
   ./tree_visualizer src/test/c/accept/06-parenthesis
   ```

4. **View the generated SVG**
   - SVG files are generated in the output directory
   - Open with any web browser or SVG editor

## Testing

The project includes a comprehensive test suite with 28 test cases covering:
- Basic arithmetic and vector operations
- Complex mathematical expressions
- Control flow structures
- Graphics primitives
- Error handling

Run all tests:
```bash
./run_tests.sh
```

## Documentation

- **Language Specification**: See `informe.md` for complete language documentation
- **Implementation Details**: Architecture and design decisions explained in the report
- **Examples**: Comprehensive examples in the `src/test/c/accept/` directory

## Recommended Development Tools

For VS Code development:

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) - Language support
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) - Build system integration
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash) - Syntax highlighting for Flex/Bison

## Contributing

This project was developed as part of a university course on Language Theory and Automata. While primarily educational, contributions and improvements are welcome.

## License

This project is developed for educational purposes. Please refer to the course guidelines for usage restrictions.
