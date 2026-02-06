# DemoLang

A simple interpreted programming language implemented in C++.

This project is mainly used to demonstrate the basic principles and implementation methods of programming language interpreters.

## Features

- **REPL Interface**: Interactive command-line environment with immediate feedback
- **Complete Pipeline**: Lexer, Parser, and Interpreter with clean separation
- **Operator Precedence**: Arithmetic, logical, and comparison operators with proper precedence
- **Basic Types**: Integer, Float, String, etc.
- **Variables**: Assignment and reference system with dynamic typing
- **Error Handling**: Comprehensive error reporting for all stages
- **Design Patterns**: Singleton, Chain of Responsibility, Visitor, Factory, Flyweight
- **Full Test Suite**: Complete test coverage with CTest integration
- **Memory Safe**: Modern C++ with smart pointers and RAII
- **Extensible**: Clean architecture for easy language extension

## Architecture

```
Source → Lexer → Tokens → Parser → AST → Interpreter → Result
```

### Core Components

- **Lexer**: Tokenizes input using Chain of Responsibility
- **Parser**: Builds AST with proper operator precedence using recursive descent
- **Interpreter**: Evaluates AST using Visitor pattern with type safety
- **Environment**: Manages variable storage and scope
- **Type System**: Integer, Float, String, Exception types

## Project Structure

```
DemoLang/
├── .gitignore
├── LICENSE
├── README.md
├── CMakeLists.txt
├── doc/                      # Documentation
│   ├── quick-start.md        # Quick start guide
│   ├── language-syntax.md    # Complete syntax reference
│   ├── code-examples.md      # Code examples
│   └── architecture.md       # Architecture overview
├── include/                  # Header files
│   ├── ast.hpp               # Abstract Syntax Tree definitions
│   ├── builtins.hpp          # Built-in functions and types
│   ├── interpreter.hpp       # Interpreter interface
│   ├── lexer.hpp             # Lexer interface
│   ├── parser.hpp            # Parser interface
│   ├── tokens.hpp            # Token definitions
│   └── utils.hpp             # Utility functions
├── src/                      # Source files
│   ├── main.cpp              # Main entry point
│   ├── CMakeLists.txt        # Source build configuration
│   ├── lexer/                # Lexer implementation
│   │   ├── lexer.cpp
│   │   └── handlers.cpp
│   ├── parser/               # Parser implementation
│   │   ├── parser.cpp
│   │   ├── operators.cpp
│   │   └── singles.cpp
│   └── interpreter/          # Interpreter implementation
│       ├── interpreter.cpp
│       ├── operator_factory.cpp
│       ├── operators.cpp
│       └── singles.cpp
└── tests/                    # Test files
    ├── CMakeLists.txt        # Test build configuration
    ├── test_framework.hpp    # Test framework
    ├── test_lexer.cpp        # Lexer tests
    ├── test_parser.cpp       # Parser tests
    ├── test_interpreter.cpp  # Interpreter tests
    └── test_utils.cpp        # Utility tests
```

## Build

### Requirements

- **Compiler**: C++17 compatible (GCC 7+, Clang 5+, MSVC 2017+)
- **Build Tool**: CMake 3.10 or higher
- **Version Control**: Git

### Build Steps

1. **Clone repository**:
   ```bash
   git clone https://github.com/The-XiaoBai/DemoLang.git
   cd DemoLang
   ```

2. **Configure and build**:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

3. **Run interpreter**:
   ```bash
   ./DemoLang          # Linux/macOS
   .\DemoLang.exe      # Windows
   ```

4. **Run tests**:
   ```bash
   ctest
   ```

## Documentation

> **[Quick Start](doc/quick-start.md)** - Get started with DemoLang

## License

This project is licensed under MIT. See [LICENSE](LICENSE) for details.