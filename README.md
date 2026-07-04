# DemoLang

> A C++ based scripting language for demonstration purposes, using the MIT license.

> This project `DemoLang` is mainly used to demonstrate the basic principles and implementation methods of interpreted programming language.

## Architecture

```
Source → Lexer → Tokens → Parser → AST → Interpreter → Result
```

## File Structure

```
DemoLang/
├── .gitattributes            # Git line-ending & language config
├── .gitignore                # Git ignore rules
├── LICENSE                   # MIT License
├── README.md                 # Project overview
├── CHANGELOG.md              # Version history
├── CONTRIBUTING.md           # Contribution guide
├── CMakeLists.txt            # Top-level build config
├── doc/                      # Documentation
│   ├── GettingStarted.md     # Quick start guide
│   ├── Variables.md          # Variables & functions
│   ├── DataCalculations.md   # Types & operators
│   ├── ControlFlows.md       # If / While statements
│   └── Details.md            # Implementation details
├── src/                      # Source files
│   ├── CMakeLists.txt        # Library + executables
│   ├── main.cpp              # REPL entry point
│   ├── fileloader.cpp        # File execution entry point
│   ├── include/              # Header files
│   │   ├── tokens.hpp        # Token definitions
│   │   ├── lexer.hpp         # Lexer interface
│   │   ├── ast.hpp           # AST node definitions
│   │   ├── parser.hpp        # Parser interface
│   │   ├── builtins.hpp      # Built-in functions & types
│   │   ├── interpreter.hpp   # Interpreter interface
│   │   └── utils.hpp         # Utility helpers
│   ├── lexer/                # Lexer implementation
│   │   ├── lexer.cpp         # Tokenizer core loop
│   │   ├── eof.cpp           # End-of-file token
│   │   ├── identifier.cpp    # Identifiers & keywords
│   │   ├── numbers.cpp       # Integer & float literals
│   │   ├── operators.cpp     # Operator & delimiter tokens
│   │   ├── string.cpp        # String literal lexing
│   │   ├── unknown.cpp       # Unknown character → error token
│   │   └── whitespaces.cpp   # Whitespace skipping (no token)
│   ├── parser/               # Parser implementation
│   │   ├── ast.cpp           # AST node constructors & toGraph()
│   │   ├── parser.cpp        # Parser core: expression chain, precedence
│   │   ├── functions.cpp     # FunctionCallParser: named function call
│   │   ├── identifier.cpp    # IdentifierParser: variable lookup
│   │   ├── lists.cpp         # ListParser: list literal & index access
│   │   ├── literals.cpp      # LiteralFallbackParser: int, float, string, bool
│   │   ├── operators.cpp     # UnaryParser, BinaryParser, PostfixParser
│   │   ├── parenthesized.cpp # ParenthesizedParser: grouping, lambda, call
│   │   └── statements.cpp    # IfParser, WhileParser, LoopControlParser
│   └── interpreter/          # Interpreter implementation
│       ├── builtins.cpp      # Built-in functions: print, exit, query
│       ├── interpreter.cpp   # Eval loop: walk AST, manage scope stack
│       ├── controlflow.cpp   # If/else, while, break, continue
│       ├── literals.cpp      # Literal evaluation
│       ├── operators.cpp     # Arithmetic, comparison, logical evaluation
│       └── variables.cpp     # Variable assignment, lookup, scope
└── tests/                    # Test suite
    ├── CMakeLists.txt        # Test build config
    ├── test_framework.hpp    # Lightweight test framework
    ├── test_operators.cpp    # Arithmetic, comparison, logical, unary operators
    ├── test_variables.cpp    # Variable assignment, lookup, scope
    ├── test_literals.cpp     # Integer, float, string, boolean literals
    ├── test_controlflow.cpp  # If, while, break, continue
    ├── test_functions.cpp    # Function definition, call, lambda, built-in functions
    ├── test_lists.cpp        # List creation, index, nested lists
    ├── test_errors.cpp       # Lexer, parser, runtime error handling
    ├── test_integration.cpp  # Cross-feature integration tests
    └── test_utils.cpp        # Utility classes unit tests
```

## Build

### Requirements

- **Compiler**: C++20 compatible (GCC 10+, Clang 10+, MSVC 2019+)
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

3. **Run executables**:
   ```bash
   ./Shell                    # Run REPL at Linux/macOS
   .\Shell.exe                # Run REPL at Windows
   ./FileLoader filename      # Execute file at Linux/macOS
   .\FileLoader.exe filename  # Execute file at Windows
   ```

4. **Run tests**:
   ```bash
   ctest
   ```

## Documentation

- **[Changelog](CHANGELOG.md)**
- **[Getting Started](doc/GettingStarted.md)**
- **[Data Types And Calculations](doc/DataCalculations.md)**
- **[Variables And Functions](doc/Variables.md)**
- **[Control Flows](doc/ControlFlows.md)**
- **[Implementation Details](doc/Details.md)**

## Contributing

First of all, thank you for your interest in contributing to DemoLang! Please read the [contribution guide](CONTRIBUTING.md) for details on how to contribute.

## License

This project is licensed under MIT. See [LICENSE](LICENSE) for details.