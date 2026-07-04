# Implementation Details

## Namespace Structure

The codebase uses a hierarchical namespace structure under the top-level `DemoLang` namespace:

```
DemoLang
├── Utils                       // Utility classes and design pattern implementations
│   ├── Handler              // Template class for Chain of Responsibility pattern
│   ├── Chain                // Template class for chaining handlers
│   ├── Singleton             // Template class for Singleton pattern
│   ├── FlyweightFactory      // Template class for Flyweight pattern
│   └── Registry              // Template class for operator/function registry
│
├── Tokens                      // Token types and token-related utilities
│   ├── TokenType               // Enum class for token types (END, OPERATOR, IDENTIFIER, etc.)
│   ├── whitespaces            // Inline constant: supported whitespace characters
│   ├── operators              // Inline constant: supported operators
│   └── Token                   // Token struct (type + value)
│
├── LexerSpace                 // Lexer module (tokenization)
│   ├── Lexer                // Main lexer class (Singleton)
│   ├── BaseHandler          // Base class for token handlers
│   ├── EOFHandler          // Handler for end-of-file
│   ├── WhitespaceHandler    // Handler for whitespace skipping
│   ├── OperatorHandler      // Handler for operators
│   ├── IdentifierHandler    // Handler for identifiers
│   ├── NumberHandler        // Handler for numbers
│   ├── StringHandler        // Handler for strings
│   └── UnknownHandler      // Handler for unknown characters
│
├── AST                         // Abstract Syntax Tree node definitions
│   ├── ASTVisitor            // Visitor interface for AST traversal
│   ├── ASTNode               // Base class for all AST nodes
│   ├── UnaryOpNode           // Unary operations (!, -)
│   ├── BinaryOpNode          // Binary operations (+, -, *, /, =, etc.)
│   ├── IdNode                // Identifier/variable reference
│   ├── IntNode               // Integer literal
│   ├── FloatNode             // Float literal
│   ├── StringNode            // String literal
│   ├── ErrorNode             // Error node for parse errors
│   ├── FunctionCallNode      // Function call
│   ├── FunctionDefNode       // Function definition
│   ├── IfNode                // If/else-if/else statement
│   ├── WhileNode             // While loop
│   ├── LoopControlType       // Enum for Break / Continue
│   ├── LoopControlNode       // Break/continue statement
│   ├── StatementSequenceNode // Sequence of statements
│   ├── ListNode              // List literal
│   └── IndexNode             // Index access (list[index])
│
├── ParserSpace                // Parser module (AST generation)
│   ├── Parser               // Main parser class (Singleton)
│   ├── BaseParser           // Base class for expression parsers
│   ├── UnaryParser          // Parser for unary operations
│   ├── BinaryParser         // Parser for binary operations
│   ├── PostfixParser        // Parser for postfix index access (expr[index])
│   ├── ListParser           // Parser for list literals
│   ├── FunctionCallParser   // Parser for function calls
│   ├── IdentifierParser     // Parser for identifiers
│   ├── IfParser             // Parser for if statements
│   ├── WhileParser          // Parser for while loops
│   ├── LoopControlParser    // Parser for break/continue
│   ├── ParenthesizedParser  // Parser for parenthesized expressions and lambdas
│   └── LiteralFallbackParser // Fallback parser for literals
│
├── ValueTypes                 // Built-in value types and functions
│   ├── BaseType             // Base class for all value types
│   ├── Integer              // Integer type (long long)
│   ├── Float                // Float type (long double)
│   ├── String               // String type (std::string)
│   ├── Exception            // Exception type for error handling
│   ├── List                 // List type (vector of BaseType)
│   └── getBuiltin           // Function to get built-in function implementations
│
└── InterpreterSpace           // Interpreter module (execution)
    ├── Environment           // Variable and function scope management
    └── Interpreter          // Main interpreter class (Singleton, ASTVisitor)
```

## Design Patterns

Several design patterns are used throughout the codebase to keep the architecture clean and extensible:

- **Chain of Responsibility**: The Lexer uses a handler chain (`WhitespaceHandler`, `StringHandler`, `NumberHandler`, etc.) where each handler attempts to process the current character; if unmatched, it passes to the next handler.
- **Singleton**: Core components (`Lexer`, `Parser`, `Interpreter`) are singletons managed via `utils.hpp`, ensuring only one instance exists per session.
- **Flyweight (Object Pool)**: Tokens are stored in a `FlyweightFactory` keyed by `type:value`. Identical tokens share a single instance, reducing memory usage.
- **Registry**: Operators and built-in functions are registered via centralized maps (`OperatorRegistry`, `BuiltinRegistry`), making it easy to add new operators or functions without modifying the core interpreter.

## Error Handling

Three-phase error detection:

- **Lexer**: invalid characters, unterminated strings, malformed numbers
- **Parser**: unexpected tokens, invalid syntax, missing brackets
- **Runtime**: division by zero, undefined variables, type errors

```
> 10 / 0
>>> Division by zero

> undefined_var
>>> Undefined variable: undefined_var
```

## Data Types (Internal)

- **Integer**: `long long` (64-bit)
- **Float**: `long double` (platform-dependent precision)
- **String**: `std::string` (UTF-8)
- **List**: `std::vector<std::shared_ptr<BaseType>>`
- **Exception**: `std::string` message — used internally for break/continue and error propagation
