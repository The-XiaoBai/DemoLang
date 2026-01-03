# Architecture

## Table of Contents
- [Overview](#overview)
- [Design Patterns](#design-patterns)
- [Lexer](#lexer)
- [Parser](#parser)
- [Interpreter](#interpreter)
- [Error Handling](#error-handling)

## Overview

DemoLang follows a classic interpreter pipeline:
```
Source → Lexer → Tokens → Parser → AST → Interpreter → Result
```

## Design Patterns

### Singleton
Ensures single instances of core components:
- Lexer, Parser, Interpreter
- Environment for variable storage

### Chain of Responsibility
Used in both lexer and parser for modular processing:
- Lexer: Token recognition handlers
- Parser: Operator precedence handlers

### Visitor Pattern
AST traversal and evaluation:
- Interpreter visits each node type
- Extensible for new operations

### Factory Pattern
Creates binary operators based on token types.

## Lexer

### Token Types
- `END` - End of input
- `OPERATOR` - +, -, *, /, ==, !=, >, <, >=, <=, =, !, &, |
- `IDENTIFIER` - Variable names
- `INTEGER_LITERAL` - Whole numbers
- `FLOAT_LITERAL` - Decimal numbers  
- `STRING_LITERAL` - Text in quotes
- `ERROR` - Invalid tokens

### Handler Chain
1. EOFHandler - End of input
2. WhitespaceHandler - Spaces, tabs, newlines
3. StringHandler - String literals
4. NumberHandler - Integers and floats
5. OperatorHandler - Mathematical operators
6. IdentifierHandler - Variable names
7. UnknownHandler - Error handling

## Parser

### AST Node Types
- `UnaryOpNode` - !x, -x
- `BinaryOpNode` - x + y, x == y
- `IdNode` - Variable identifiers
- `IntNode` - Integer literals
- `FloatNode` - Float literals
- `StringNode` - String literals
- `ErrorNode` - Parse errors

### Parser Chain (by precedence)
1. AssignmentParser - `=`
2. LogicalParser - `&`, `|`
3. EqualityParser - `==`, `!=`
4. RelationalParser - `<`, `<=`, `>`, `>=`
5. AdditiveParser - `+`, `-`
6. MultiplicativeParser - `*`, `/`
7. UnaryParser - `!`, `-`
8. PrimaryParser - Literals, identifiers, parentheses

## Interpreter

### Type System
- `Integer` - 64-bit signed integers
- `Float` - 80-bit floating point
- `String` - Variable-length text
- `Exception` - Error values

### Environment
Manages variable storage and scope:
- Variable name → value mapping
- Support for reassignment
- Type safety checks

### Evaluation Process
1. Visit AST nodes in order
2. Evaluate expressions recursively
3. Handle type conversions
4. Manage variable state
5. Return final result

## Error Handling

### Lexer Errors
- Invalid characters
- Unterminated strings
- Malformed numbers

### Parser Errors
- Unexpected tokens
- Invalid syntax
- Missing parentheses

### Runtime Errors
- Division by zero
- Type mismatches
- Undefined variables

## Testing

### Test Structure
- `test_lexer.cpp` - Token recognition tests
- `test_parser.cpp` - AST construction tests  
- `test_interpreter.cpp` - Evaluation tests
- `test_utils.cpp` - Utility function tests

### Running Tests
```bash
cd build
ctest              # All tests
./test_lexer        # Lexer only
./test_parser       # Parser only
./test_interpreter  # Interpreter only
```