# DemoLang Documentation

## Table of Contents
- [Getting Started](#getting-started)
- [Language Reference](#language-reference)
- [Built-in Functions](#built-in-functions)
- [Error Handling](#error-handling)
- [Examples](#examples)

## Getting Started

DemoLang is a simple interpreted programming language. Start the REPL:

```bash
./Shell          # Run REPL at Linux/macOS
.\Shell.exe      # Run REPL at Windows
./FileLoader filename      # Execute file at Linux/macOS
.\FileLoader.exe filename  # Execute file at Windows
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

**Operators**: `+ - * / == != > < >= <= = ! & | ( ) [ ] @ ;`

### Data Types

- **Integer**: 64-bit signed integer
- **Float**: 80-bit floating point
- **String**: UTF-8 encoded
- **List**: Dynamic array of mixed types, supports nesting

### List

Lists are ordered collections enclosed in brackets:

```
nums = [1, 2, 3]           # List of integers
mixed = [1, "hello", 3.14] # Mixed types
nested = [1, [2, 3], [4]]  # Nested lists

# Index access (0-based)
print(nums[0])             # Output: 1
print(nums[2])             # Output: 3

# Chained index access
print(nested[1][0])        # Output: 2
print(nested[2][0])        # Output: 4
```

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

### Control Flow

#### If Statement

DemoLang supports conditional execution with if-else syntax:

```
# Basic if
?(condition) { body }

# If-else
?(condition) { body } : { else_body }

# If-else if-else
?(condition) { body } ??(condition) { body } : { else_body }
```

**Syntax**:
- `?` starts an if branch
- `??` starts an else-if branch (multiple allowed)
- `:` introduces the else branch (optional)
- Conditions are evaluated sequentially; first true condition executes its body
- Truthy values: non-zero integers, non-zero floats, non-empty strings
- Falsy values: `0`, `0.0`, `""`

```
x = 5
?(x > 10) { print("large") } : { print("small") }
# Output: small

y = 15
?(y > 10) { print("A") } ??(y > 5) { print("B") } : { print("C") }
# Output: A
```

#### While Loop

DemoLang supports while loops with break and continue statements:

```
# Basic while loop
$(condition) { body }

# Break statement (exits loop)
$(condition) { ## }

# Continue statement (skips to next iteration)
$(condition) { # }
```

**Syntax**:
- `$` starts a while loop
- Condition is evaluated before each iteration
- Loop continues while condition is truthy
- `##` (break) exits the loop immediately
- `#` (continue) skips remaining body and re-evaluates condition

```
# Simple counter
i = 1
$(i <= 5) { print(i); i = i + 1 }
# Output: 1 2 3 4 5

# Break example
j = 1
$(j <= 10) { 
    ?(j > 5) { ## }  # Break when j > 5
    print(j); j = j + 1 
}
# Output: 1 2 3 4 5

# Continue example
k = 0
$(k < 10) {
    k = k + 1
    ?(k % 2 == 0) { # }  # Skip even numbers
    print(k)
}
# Output: 1 3 5 7 9
```

### Statement Separation

DemoLang uses semicolon (`;`) to separate multiple statements on a single line or within a block:

```
# Multiple statements on one line with semicolon
x = 10; y = 20; print(x + y)  # Output: 30

# Statements in control flow blocks
counter = 0
$(counter < 3) { 
    print(counter); 
    counter = counter + 1 
}
# Output: 0 1 2

# Complex example with mixed statements
a = 5; b = 10
$(a < b) { 
    print("a:", a); 
    a = a + 1; 
    # break when a reaches 8
    ?(a == 8) { ## } 
}
# Output: a: 5 a: 6 a: 7
```

**Rules**:
- `;` separates statements within the same line
- Statements in blocks (curly braces) can be separated by `;` or newlines
- The last statement in a line or block does not require `;`
- `;` can be used in REPL to execute multiple commands at once

### Operators

**Arithmetic**: `+`, `-`, `*`, `/`

**Comparison**: `==`, `!=`, `>`, `<`, `>=`, `<=`

**Logical**: `&` (AND), `|` (OR), `!` (NOT)

**Return**: `@` (return value from function)

**Assignment**: `=`

**Precedence** (highest to lowest):
1. `!`, `-` (unary)
2. `*`, `/`
3. `+`, `-`
4. `<`, `<=`, `>`, `>=`
5. `==`, `!=`
6. `&`, `|`
7. `@` (return)
8. `=`

## Built-in Functions

DemoLang provides several built-in functions for common operations:

### print

Prints values to the console, separated by spaces, followed by a newline.

```
print(42)                    # Output: 42
print(3.14)                  # Output: 3.140000
print("Hello")               # Output: Hello
print(1, 2.5, "test")        # Output: 1 2.500000 test
```

### exit

Exits the program with an optional exit code (default: 0).

```
exit()                       # Exit with code 0
exit(1)                      # Exit with code 1
```

### query

Reads a line of input from the console and returns it as a string.

```
name = query()               # Reads user input
print("Hello, " + name)      # Greets the user
```

## User-Defined Functions

DemoLang supports user-defined functions with the following syntax:

### Function Definition

Functions are defined using assignment to lambda expressions:

```
# Basic function without parameters
greet = () { @ "Hello, World!" }

# Function with parameters
add = (a, b) { @ a + b }

# Function with default parameters
greet = (name = "World") { @ "Hello, " + name + "!" }
```

**Lambda Syntax**: `(parameters) { @ return_value }`
- Parameters are enclosed in parentheses
- Default values can be specified using `=` (e.g., `x = 10`)
- Function body is enclosed in curly braces
- **`@` operator is required to return a value**
- Functions without `@` execute the body but return empty string
- Functions without `@` can still contain statements (e.g., `print`)

### Lambda Functions

DemoLang supports anonymous lambda functions that can be:
1. Assigned to variables for later use
2. Immediately executed with arguments
3. Passed as values (first-class functions)

```
# Lambda assigned to a variable
square = (x) { @ x * x }
result = square(5)           # result = 25

# Lambda with immediate execution
result = (a, b) { @ a + b }(10, 20)  # Returns 30
```

### Function Calls

Functions are called using positional arguments:

```
# Call without arguments
greet()                      # Returns: Hello, World!

# Call with arguments
add(10, 20)                  # Returns 30

# Override default parameter
greet("Alice")               # Returns: Hello, Alice!
```

### Default Parameters

Functions can specify default values for parameters:

```
# Function with default parameter
calc = (a = 10, b = 20) { @ a + b }

# Call with defaults
calc()                       # Returns 30

# Override defaults
calc(5, 3)                   # Returns 8
```

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

### File Execution
Save the following code to a file named `demo` for example:
```
x = 10
y = 20
result = x + y
name = "DemoLang"
greeting = "Hello, " + name
print(greeting)
```
Then run the file loader:
```
./FileLoader demo
```
The output should be `Hello, DemoLang` in the end.