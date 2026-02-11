# DemoLang Documentation

## Table of Contents
- [Getting Started](#getting-started)
- [Language Reference](#language-reference)
- [Error Handling](#error-handling)
- [Examples](#examples)

## Getting Started

DemoLang is a simple interpreted programming language. Start the REPL:

```bash
./DemoLang          # Linux/macOS
.\DemoLang.exe      # Windows
```

```
[DemoLang]

> 1000 + 24
>>> 1024

> name = "DemoLang"
>>> DemoLang

> "Hi, " + name + "!"
>>> Hi, DemoLang!
```

## Language Reference

### Tokens

**Literals**
- Integer: `42`, `-17`, `0`
- Float: `3.14`, `-0.5`, `10.0`
- String: `"Hello"`, `"DemoLang"`

**Identifiers**: Variable names with alphanumeric characters and underscores: `x`, `variable_name`, `value123`

**Operators**: `+ - * / == != > < >= <= = ! & | ( )`

### Data Types

- **Integer**: 64-bit signed integer
- **Float**: 80-bit floating point
- **String**: UTF-8 encoded

### Variables

Variables are dynamically typed and created upon first assignment:

```
x = 42           # Integer
name = "Demo"    # String
pi = 3.14        # Float

x = 3.14         # Reassign to different type
```

### Expressions

**Primary**: literals, variables

**Unary**: `-5`, `!1` (logical NOT)

**Binary**: `10 + 5`, `20 - 8`, `6 * 7`, `10 / 3`

**Parenthesized**: `(10 + 5) * 2`

**String**: `"Hello" + " " + "World"`

### Operators

**Arithmetic**: `+`, `-`, `*`, `/`

**Comparison**: `==`, `!=`, `>`, `<`, `>=`, `<=`

**Logical**: `&` (AND), `|` (OR), `!` (NOT)

**Assignment**: `=`

**Precedence** (highest to lowest):
1. `!`, `-` (unary)
2. `*`, `/`
3. `+`, `-`
4. `<`, `<=`, `>`, `>=`
5. `==`, `!=`
6. `&`, `|`
7. `=`

## Error Handling

**Lexer Errors**: Invalid characters, unterminated strings, malformed numbers, etc.

**Parser Errors**: Unexpected tokens, invalid syntax, missing parentheses, etc.

**Runtime Errors**: Division by zero, invalid operands, undefined variables, etc.

```
> 10 / 0
>>> Division by zero

> undefined_var
>>> Undefined variable: undefined_var
```

## Examples

### Basic Operations
```
result = 10 + 5 * 2        # 20
result = (10 + 5) * 2      # 30
result = 100 - 20 / 5      # 96

> 3.14 * 2
>>> 6.280000

> 10 > 5
>>> 1

> (10 > 5) & (3 < 8)
>>> 1
```

### String Operations
```
first_name = "John"
last_name = "Doe"
full_name = first_name + " " + last_name
greeting = "Hello, " + full_name + "!"
```

### Variable Patterns
```
# Counter
counter = 0
counter = counter + 1           # 1
counter = counter * 2           # 2

# Accumulator
sum = 0
sum = sum + 10                  # 10
sum = sum + 20                  # 30
```

### Type Conversion
```
int_val = 10
float_val = 3.14
result = int_val + float_val    # 13.140000 (float)

num = 42
is_truthy = !num                # 0 (false)
empty = 0
is_falsy = !empty               # 1 (true)
```