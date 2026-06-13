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
│   │   ├── lexer.cpp
│   │   ├── eof.cpp
│   │   ├── identifier.cpp
│   │   ├── numbers.cpp
│   │   ├── operators.cpp
│   │   ├── string.cpp
│   │   ├── unknown.cpp
│   │   └── whitespaces.cpp
│   ├── parser/               # Parser implementation
│   │   ├── ast.cpp
│   │   ├── parser.cpp
│   │   ├── operators.cpp
│   │   ├── literals.cpp
│   │   ├── lists.cpp
│   │   ├── parenthesized.cpp
│   │   ├── functions.cpp
│   │   ├── identifier.cpp
│   │   └── statements.cpp
│   └── interpreter/          # Interpreter implementation
│       ├── builtins.cpp
│       ├── interpreter.cpp
│       ├── operators.cpp
│       ├── literals.cpp
│       ├── variables.cpp
│       └── controlflow.cpp
└── tests/                    # Test suite
    ├── CMakeLists.txt        # Test build config
    ├── test_framework.hpp    # Lightweight test framework
    ├── test_lexer.cpp        # Lexer tests
    ├── test_parser.cpp       # Parser tests
    ├── test_interpreter.cpp  # Interpreter tests
    └── test_utils.cpp        # Utility tests
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