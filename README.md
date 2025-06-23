[![✗](https://img.shields.io/badge/Release-v1.0.0-ffb600.svg?style=for-the-badge)](https://github.com/agustin-golmar/Flex-Bison-Compiler/releases)

# 🎨 Dibujitos

> **A Domain-Specific Language for Creating Beautiful SVG Graphics Through Code**

Transform mathematical expressions and geometric concepts into stunning visual art with Dibujitos - a programming language designed to bridge the gap between code and creativity.

---

## ✨ What is Dibujitos?

Dibujitos is a domain-specific language that makes creating 2D vector graphics as simple as writing mathematical expressions. Whether you're an educator teaching programming concepts, an artist exploring generative design, or a developer prototyping visualizations, Dibujitos provides an intuitive way to express visual ideas through code.

### 🚀 Key Features

| Feature | Description |
|---------|-------------|
| 🧮 **Mathematical Foundation** | Built-in trigonometric, logarithmic, and arithmetic functions |
| 📐 **Vector Operations** | Full 2D vector arithmetic and manipulation |
| 🎯 **Geometric Primitives** | Circles, lines, Bézier curves, and polygons |
| 🔄 **Smart Iteration** | Arrays, ranges, and for-each loops |
| 📝 **Dynamic Text** | String interpolation with variable substitution |
| 🧠 **Type Intelligence** | Static typing with automatic type inference |
| 🖼️ **SVG Output** | High-quality scalable vector graphics |
| ⚙️ **Configurable Canvas** | Custom dimensions and styling options |

---

## 🎯 Quick Start

### ⚡ Installation

Choose your preferred method:

<details>
<summary>🐳 Docker (Recommended)</summary>

See detailed [Docker installation guide](doc/readme/Docker.md)
</details>

<details>
<summary>🐧 Linux/Ubuntu</summary>

See detailed [Linux/Ubuntu installation guide](doc/readme/Ubuntu.md)
</details>

<details>
<summary>🪟 Windows</summary>

See detailed [Windows installation guide](doc/readme/Windows.md)
</details>

### 🎨 Your First Drawing

Create a file called `hello.dib`:

```javascript
// A simple colorful spiral
fill(0xFF6B6B);  // Coral red
stroke(0x4ECDC4); // Turquoise

for i in [1:8] {
    angle = i * 0.8;
    radius = i * 12.0;
    pos_x = cos(angle) * radius;
    pos_y = sin(angle) * radius;
    circle((pos_x, pos_y), i * 3.0);
}
```

Generate your SVG using the provided scripts:

```bash
# Linux/Mac
./script/ubuntu/start.sh hello.dib -w 600 -h 600

# Windows  
.\script\windows\start.bat hello.dib -w 600 -h 600
```

---

## 📖 Language Guide

### 📊 Data Types at a Glance

```javascript
// Numbers
age = 42;              // Integer
pi = 3.14159;          // Float
color = 0xFF5733;      // Hexadecimal

// Vectors (2D coordinates)
origin = (0, 0);
point = (100, 200);

// Arrays
colors = [0xFF0000, 0x00FF00, 0x0000FF];  // RGB colors
range = [1:10];                           // Numbers 1 through 10

// Strings with interpolation
message = "Point is at {point.pos_x}, {point.pos_y}";
```

### 🎨 Drawing Functions

```javascript
// Basic shapes
circle((50, 50), 25);              // Circle at (50,50) with radius 25
line((0, 0), (100, 100));          // Line from origin to (100,100)
curve((0,0), (50,100), (50,0), (100,100)); // Bézier curve

// Styling
fill(0xFF6B6B);                    // Set fill color
stroke(0x333333);                  // Set stroke color
layer(FRONT);                      // Set drawing layer
```

### 🧮 Mathematical Power

```javascript
// Trigonometry
angle = pi / 4;
pos_x = cos(angle) * 50;
pos_y = sin(angle) * 50;

// Advanced math
distance = sqrt(pos_x*pos_x + pos_y*pos_y);
rounded = roundf(distance);
logarithm = logf(10.0);
power = powf(2.0, 8.0);
```

### 🔄 Control Flow

```javascript
// Conditionals
if (radius > 50) {
    fill(0xFF0000);  // Red for large circles
} else {
    fill(0x0000FF);  // Blue for small circles
}

// Iteration
for color in [0xFF0000, 0x00FF00, 0x0000FF] {
    fill(color);
    circle((0, 0), radius);
    radius = radius + 10;
}
```

---

## 🛠️ Command Line Interface

### Basic Usage

Use the provided scripts for the best experience:

```bash
# Linux/Mac - Output to file
./script/ubuntu/start.sh input.dib -o output.svg

# Linux/Mac - Custom canvas size
./script/ubuntu/start.sh input.dib -w 1920 -h 1080 -o poster.svg

# Windows - Output to file
.\script\windows\start.bat input.dib -o output.svg

# Windows - Custom canvas size
.\script\windows\start.bat input.dib -w 1920 -h 1080 -o poster.svg
```

### 📋 All Options

| Option | Description | Default |
|--------|-------------|---------|
| `-o <file>` | Output SVG file | stdout |
| `-w <width>`, `--width <width>` | Canvas width | 1000 |
| `-h <height>`, `--height <height>` | Canvas height | 1000 |

---

## 🔍 Development Tools

### Tree Visualizer

Inspect how your code is parsed and analyzed:

```bash
./tree_visualizer examples/spiral.dib
```

**Output includes:**
- 🌳 **AST Structure** - Visual representation of your code's structure
- 📋 **Symbol Table** - All variables with types and values  
- 🔍 **Type Information** - Detailed type analysis
- 🐛 **Debug Info** - Helpful for troubleshooting

### Environment Configuration

Fine-tune the compiler behavior:

```bash
export LOGGING_LEVEL=DEBUGGING     # Show detailed logs
export SHOW_AST=true              # Display AST in console
export LOG_IGNORED_LEXEMES=false  # Reduce verbose output
```

---

## ✅ Testing & Quality

### Run the Test Suite

```bash
# Run all 28+ test cases
./script/ubuntu/test.sh

# Test a specific feature
./tree_visualizer src/test/c/accept/28-math-functions
```

### Test Categories

- ✅ **Arithmetic Operations** - Basic math and operator precedence
- ✅ **Vector Mathematics** - 2D coordinate manipulation  
- ✅ **Control Structures** - Conditionals and loops
- ✅ **Graphics Primitives** - Shapes and styling
- ✅ **Type System** - Type inference and checking
- ✅ **Error Handling** - Graceful failure modes

---

## 🎯 Use Cases & Examples

### 📚 Educational
```javascript
// Visualize trigonometric functions
for i in [0:360] {
    angle = i * pi / 180;
    pos_x = i * 2;
    pos_y = sin(angle) * 100;
    circle((pos_x, pos_y), 2);
}
```

### 🎨 Generative Art
```javascript
// Fractal-inspired pattern
for layer in [1:5] {
    for angle in [0:12] {
        theta = angle * pi / 6;
        r = layer * 20;
        fill(layer * 0x333333);
        circle((cos(theta) * r, sin(theta) * r), layer * 5);
    }
}
```

### 📊 Data Visualization
```javascript
// Simple bar chart
data = [23, 45, 56, 78, 32];
for i in [0:4] {
    height = data[i] * 3;
    fill(0x4A90E2);
    polygon([(i*50, 0), (i*50+40, 0), (i*50+40, height), (i*50, height)]);
}
```

---

## 👥 Team

<table>
<tr>
<td align="center">
<strong>Juan Ignacio Cantarella</strong><br>
<code>64509</code><br>
📧 jcantarella@itba.edu.ar
</td>
<td align="center">
<strong>Máximo Daniel Carranza</strong><br>
<code>64538</code><br>
📧 mcarranza@itba.edu.ar
</td>
</tr>
<tr>
<td align="center">
<strong>Alex Köhler</strong><br>
<code>64320</code><br>
📧 akohler@itba.edu.ar
</td>
<td align="center">
<strong>Javier Liu</strong><br>
<code>64332</code><br>
📧 jaliu@itba.edu.ar
</td>
</tr>
</table>

---

## 📚 Documentation

| Resource | Description |
|----------|-------------|
| 📖 [Language Specification](informe.md) | Complete syntax and semantics |
| 🏗️ [Architecture Guide](doc/architecture.md) | Implementation details |
| 🎯 [Examples Gallery](src/test/c/accept/) | 28+ working examples |
| 🐳 [Docker Setup](doc/readme/Docker.md) | Containerized development |
| 🪟 [Windows Guide](doc/readme/Windows.md) | Windows-specific setup |
| 🐧 [Ubuntu Guide](doc/readme/Ubuntu.md) | Linux development |

---

## 🤝 Contributing

This project was developed for the **Language Theory and Automata** course. While primarily educational, we welcome:

- 🐛 Bug reports and fixes
- 📝 Documentation improvements  
- ✨ Feature suggestions
- 🎨 Example contributions

---

## 📄 License

Educational project developed at ITBA. See course guidelines for usage terms.

---

<div align="center">

**Made with ❤️ by the Dibujitos Team**

*Transform code into art, one vector at a time*

</div>
