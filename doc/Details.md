# Implementation Details

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
