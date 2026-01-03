# Language Syntax

## Table of Contents
- [Data Types](#data-types)
- [Operators](#operators)
- [Variables](#variables)
- [Expressions](#expressions)

## Data Types

### Integer
64-bit signed integers.
```
42
-17
0
```

### Float
80-bit floating-point values.
```
3.14
-0.5
10.0
```

### String
Text enclosed in double quotes.
```
"Hello"
"DemoLang"
"Hello, World!"
```

## Operators

### Arithmetic
- `+` - Addition
- `-` - Subtraction  
- `*` - Multiplication
- `/` - Division (returns float)

### Comparison
- `==` - Equal
- `!=` - Not equal
- `>` - Greater than
- `<` - Less than
- `>=` - Greater or equal
- `<=` - Less or equal

### Logical
- `&` - AND
- `|` - OR
- `!` - NOT

### Assignment
- `=` - Variable assignment

### Operator Precedence

From highest to lowest:
1. `!` (NOT), `-` (unary)
2. `*`, `/`
3. `+`, `-`
4. `<`, `<=`, `>`, `>=`
5. `==`, `!=`
6. `&`, `|`
7. `=` (assignment)

## Variables

Variables are dynamically typed and can be reassigned:
```
x = 42           # Integer
x = 3.14         # Float
x = "Hello"      # String
```

## Expressions

All expressions return a value:
```
> 10 + 5 * 2     # Operator precedence
>>> 20

> (10 + 5) * 2   # Parentheses
>>> 30

> "Hi" + " " + "there"  # String concatenation
>>> Hi there
```