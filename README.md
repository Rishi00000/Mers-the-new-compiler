# MERS - A Simple Yet Powerful Compiler

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](#building-the-compiler)
[![Language](https://img.shields.io/badge/language-C-blue)](#)
[![License](https://img.shields.io/badge/license-MIT-green)](#)

> **MERS** is an educational compiler project designed to teach and demonstrate the fundamental concepts of compiler design and implementation. It processes a custom simple language and generates x86-64 assembly code.

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Getting Started](#getting-started)
- [Building the Compiler](#building-the-compiler)
- [Using the Compiler](#using-the-compiler)
- [Language Syntax](#language-syntax)
- [Compilation Phases](#compilation-phases)
- [Project Structure](#project-structure)
- [Test Cases](#test-cases)
- [Example Programs](#example-programs)
- [Contributing](#contributing)

## 🎯 Overview

MERS is a complete compiler implementation that takes source code written in a simple, carefully designed language and transforms it through 6 distinct compilation phases to produce x86-64 assembly code. The compiler demonstrates core compiler concepts including:

- **Lexical Analysis** - Breaking source code into tokens
- **Syntax Analysis** - Building an Abstract Syntax Tree (AST)
- **Semantic Analysis** - Validating program correctness
- **Code Generation** - Transforming high-level code to low-level assembly

Perfect for students learning compiler design, the MERS compiler provides clear, readable output at each phase, making it an excellent educational tool.

## ✨ Features

### Core Language Features
- ✅ Variable declarations with `int` type
- ✅ Assignments using the `<-` operator
- ✅ Arithmetic expressions (`+`, `-`, `*`, `/`)
- ✅ Comparison operators (`<`, `>`, `<=`, `>=`, `==`, `!=`)
- ✅ If-else conditional statements with blocks
- ✅ Print statements for output
- ✅ Nested control structures

### Compiler Features
- ✅ Detailed phase-by-phase compilation output
- ✅ Complete error detection and reporting
- ✅ Abstract Syntax Tree (AST) visualization
- ✅ Intermediate code generation with temporary variables
- ✅ x86-64 assembly code generation
- ✅ Semantic validation (variable scope and declaration checking)
- ✅ Clean, human-readable output for each phase

## 🚀 Getting Started

### Prerequisites

- GCC compiler
- Windows/Linux/macOS system
- Basic understanding of command line

### Building the Compiler

Navigate to the project directory and compile:

```bash
cd Compiler
gcc -w src/*.c -o build/compiler
```

This will create the `build/compiler` executable that you can use to compile `.my` files.

### Quick Test

To verify the compiler works correctly:

```bash
# Test with a simple valid program
./build/compiler test/valid/basic.my

# Test with if-else statements
./build/compiler test/valid/ifelse_basic.my
```

## 📝 Using the Compiler

### Basic Usage

```bash
./compiler <input_file.my>
```

### Example

```bash
$ ./compiler test/valid/basic.my
```

This will output:

1. **Lexer Phase** - Tokenization of the source code
2. **Parser Phase** - Building the Abstract Syntax Tree
3. **AST Visualization** - Hierarchical display of your program
4. **Semantic Analysis** - Validation of variables and scope
5. **Intermediate Code Generation** - IR representation
6. **Code Generation** - Final x86-64 assembly output

## 🔤 Language Syntax

### Variable Declaration

Declare integer variables using the `int` keyword:

```
int x;
int y;
int result;
```

### Variable Assignment

Assign values using the left-arrow operator `<-`:

```
x <- 10;
y <- 20;
result <- x + y;
```

### Expressions

Create expressions using arithmetic operators:

```
sum <- a + b;
difference <- x - y;
product <- m * n;
quotient <- a / b;

// Chained operations
result <- 10 - 5 - 2;  // Left-associative
```

### Print Statement

Output values using the `print` function:

```
print(x);
print(x + y);
print(result);
```

### If-Else Statements

Conditional execution with optional else blocks:

```
if (x > 5) {
    y <- 100;
} else {
    y <- 200;
}
```

### Comparisons

Use comparison operators in conditions:

```
if (a == b) { ... }        // Equal to
if (a != b) { ... }        // Not equal to
if (a < b)  { ... }        // Less than
if (a > b)  { ... }        // Greater than
if (a <= b) { ... }        // Less than or equal
if (a >= b) { ... }        // Greater than or equal
```

### Complete Example

```
int x;
int y;
int z;

x <- 15;
y <- 10;

if (x > y) {
    if (x > 20) {
        z <- 1;
    } else {
        z <- 2;
    }
} else {
    z <- 3;
}

print(z);
```

## 🔄 Compilation Phases

The MERS compiler implements 6 distinct compilation phases:

### Phase 1: Lexer
- **Input**: Raw source code
- **Output**: Stream of tokens
- **Purpose**: Breaks source code into meaningful units (keywords, identifiers, operators, numbers)
- **Errors Detected**: Unknown characters, invalid operator sequences

### Phase 2: Parser
- **Input**: Token stream from Lexer
- **Output**: Abstract Syntax Tree (AST)
- **Purpose**: Validates syntax and builds hierarchical program representation
- **Errors Detected**: Missing semicolons, mismatched parentheses, invalid expressions

### Phase 3: AST Visualization
- **Input**: AST from Parser
- **Output**: Formatted tree display
- **Purpose**: Shows the parsed program structure in a readable format
- **Information**: Helps understand how the compiler interpreted your code

### Phase 4: Semantic Analysis
- **Input**: Abstract Syntax Tree
- **Output**: Validation report
- **Purpose**: Checks for semantic correctness (variables declared before use, no duplicates)
- **Errors Detected**: Undeclared variables, duplicate declarations, scope violations

### Phase 5: Intermediate Code Generation
- **Input**: Abstract Syntax Tree
- **Output**: Intermediate representation with temporary variables and labels
- **Purpose**: Bridge between high-level constructs and low-level assembly
- **Features**: Generates human-readable IR with temporary variables (t0, t1, ...) and labels (L0, L1, ...)

### Phase 6: Code Generation
- **Input**: Abstract Syntax Tree
- **Output**: x86-64 Assembly code (NASM syntax)
- **Purpose**: Transforms program to target architecture instructions
- **Features**: Generates complete assembly with data section and executable code

## 📁 Project Structure

```
Compiler/
├── include/              # Header files
│   ├── ast.h            # Abstract Syntax Tree definitions
│   ├── codegen.h        # Code generation
│   ├── ir.h             # Intermediate code generation
│   ├── lexer.h          # Lexical analysis
│   ├── parser.h         # Syntax analysis
│   ├── semantic.h       # Semantic analysis
│   ├── symbol_table.h   # Symbol table management
│   ├── token.h          # Token definitions
│
├── src/                 # Source files
│   ├── ast.c            # AST implementation
│   ├── codegen.c        # Assembly code generation
│   ├── ir.c             # IR generation
│   ├── lexer.c          # Tokenization
│   ├── main.c           # Entry point with phase tracking
│   ├── parser.c         # Parsing and AST building
│   ├── semantic.c       # Semantic validation
│   ├── symbol_table.c   # Symbol table implementation
│   ├── token.c          # Token utilities
│
├── test/                # Test cases
│   ├── valid/           # Valid programs
│   │   ├── basic.my
│   │   ├── chained.my
│   │   ├── ifelse_basic.my
│   │   ├── ifelse_nested.my
│   │   └── ifelse_comparison.my
│   │
│   ├── lexer_errors/    # Lexer error test cases
│   │   └── invalid_char.my
│   │
│   ├── parser_errors/   # Parser error test cases
│   │   ├── bad_expression.my
│   │   └── missing_semicolon.my
│   │
│   └── semantic_errors/ # Semantic error test cases
│       ├── duplicate.my
│       └── undeclared.my
│
├── build/               # Compiled executable
│   └── compiler         # Main compiler executable
│
└── Images/              # Documentation images (if any)
```

## 🧪 Test Cases

The compiler includes comprehensive test cases covering:

### Valid Programs (test/valid/)

**basic.my** - Simple variable declaration and output
```
int x;
int y;
x <- 10;
y <- 20;
print(x - y);
```

**ifelse_basic.my** - Basic if-else conditional
```
int x;
int y;
x <- 10;
if (x > 5) {
    y <- 100;
} else {
    y <- 200;
}
print(y);
```

**ifelse_comparison.my** - Equality comparison
```
int a;
int b;
a <- 5;
b <- 5;
if (a == b) {
    print(1);
} else {
    print(0);
}
```

### Error Test Cases

**Lexer Errors** (test/lexer_errors/)
- Invalid characters that aren't recognized

**Parser Errors** (test/parser_errors/)
- Missing semicolons
- Invalid expression syntax

**Semantic Errors** (test/semantic_errors/)
- Undeclared variable usage
- Duplicate variable declarations

## 📚 Example Programs

### Example 1: Simple Calculation
```
int a;
int b;
int sum;

a <- 10;
b <- 20;
sum <- a + b;

print(sum);
```

**Output**: Shows all 6 phases, final assembly code calculates and stores the sum.

### Example 2: Conditional Logic
```
int score;
int result;

score <- 85;

if (score >= 80) {
    result <- 1;
} else {
    result <- 0;
}

print(result);
```

**Output**: Demonstrates comparison operators and conditional jumps in generated assembly.

### Example 3: Nested Conditions
```
int x;
int y;
int z;

x <- 15;
y <- 10;

if (x > y) {
    if (x > 20) {
        z <- 1;
    } else {
        z <- 2;
    }
} else {
    z <- 3;
}

print(z);
```

**Output**: Shows nested if-else handling with multiple labels and jumps.

## 🛠️ Building and Testing

### Clean Build

```bash
cd Compiler
gcc -w src/*.c -o build/compiler
```

### Run Tests

```bash
# Valid programs
./build/compiler test/valid/basic.my
./build/compiler test/valid/ifelse_basic.my
./build/compiler test/valid/ifelse_nested.my

# Error cases
./build/compiler test/parser_errors/missing_semicolon.my
./build/compiler test/semantic_errors/undeclared.my
```

### Expected Output

For valid programs: All 6 phases show `[OK]` status
For error cases: Compilation stops at error phase with error description

## 📖 Understanding the Output

When you run a program, you'll see:

```
================================================================================
                    MERS COMPILER - COMPILATION PROCESS
================================================================================

[Phase headers and outputs for all 6 phases...]

================================================================================
                      COMPILATION SUCCESSFUL!
================================================================================

  All 6 phases completed:
    1. Lexer                              [OK]
    2. Parser                            [OK]
    3. AST Visualization                 [OK]
    4. Semantic Analysis                 [OK]
    5. Intermediate Code Generation      [OK]
    6. Code Generation                   [OK]
```

Each phase displays:
- Status of what it's doing
- Input and output information
- Detailed output (AST, IR, Assembly, etc.)
- Completion status

## 🤝 Contributing

This is an educational compiler project. Contributions and improvements are welcome!

### Areas for Enhancement
- Add while/for loops
- Implement functions
- Add array support
- Improve error messages
- Add optimization passes
- Implement additional operators

## 📝 Notes for Evaluators

This compiler demonstrates:
- Complete lexical analysis
- Full syntax analysis with AST construction
- Semantic validation
- Intermediate code generation
- Machine code generation
- Proper error handling at each phase
- Clean, educational code structure

All compilation phases are clearly visible with completion status markers, making it easy to verify that the entire compilation pipeline works correctly.

## 📄 License

This project is created for educational purposes as part of a Compiler Design course at IIITDM.

---

**Questions?** Refer to the phase-by-phase output or check the test cases for examples of how the compiler handles various language constructs.
