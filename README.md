[![✗](https://img.shields.io/badge/Release-v1.0.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# Dibujitos - Language design and implementation

## Project Description

Dibujitos is a domain-specific language (DSL) designed for creating 2D drawings using geometric figures and mathematical expressions. The language aims to provide a simple yet expressive syntax that allows users to define designs, patterns, and figures. A key feature is the ability to name points for reuse throughout the design.

The output is generated in SVG format, ensuring compatibility with web browsers, graphic editors, and other SVG-supporting tools. This DSL bridges the gap between programming and visual design, making it valuable for educational, artistic, and technical contexts where graphical expression is needed.

### Tree Visualizer

The tree visualizer is a key component of this project that provides a graphical representation of the abstract syntax tree. It helps developers and users to:

- Visualize the structure of parsed code
- Debug syntax analysis issues
- Understand the hierarchical relationships between different code elements
- Validate the correctness of the parser implementation

The visualizer generates an interactive tree diagram that can be viewed in console, making it easier to navigate and inspect the AST structure.

## Team Members

| Name | Last Name | ID | Email |
|------|-----------|-----|-------|
| Juan Ignacio | Cantarella | 64509 | jcantarella@itba.edu.ar |
| Máximo Daniel | Carranza | 64538 | mcarranza@itba.edu.ar |
| Alex | Köhler | 64320 | akohler@itba.edu.ar |
| Javier | Liu | 64332 | jaliu@itba.edu.ar |

## Environment

Set the following environment variables to control and configure the behaviour of the application:

|Name|Default|Description|
|-|:-:|-|
|`LOG_IGNORED_LEXEMES`|`true`|When `true`, logs all of the ignored lexemes found with Flex at DEBUGGING level. To remove those logs from the console output set it to `false`.|
|`LOGGING_LEVEL`|`INFORMATION`|The minimum level to log in the console output. From lower to higher, the available levels are: `ALL`, `DEBUGGING`, `INFORMATION`, `WARNING`, `ERROR` and `CRITICAL`.|
|`SHOW_AST`|`false`|When `true`, enables the visualization of the Abstract Syntax Tree (AST) in the console. Set to `false` by default.|

## Installation

* [Docker](doc/readme/Docker.md)
* [Microsoft Windows](doc/readme/Windows.md)
* [Ubuntu](doc/readme/Ubuntu.md)

## Recommended Extensions

* [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
* [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
* [Yash](https://marketplace.visualstudio.com/items?itemName=daohong-emilio.yash)
