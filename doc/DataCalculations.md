# Data Types and Calculations

## Tokens

**Literals**
- Integer: `42`, `-17`, `0`
- Float: `3.14`, `-0.5`, `10.0`
- String: `"Hello"`, `"DemoLang"`

**Identifiers**: Variable names with alphanumeric characters and underscores: `x`, `variable_name`, `value123`

**Expression Operators**: `+ - * / == != > < >= <= = ! & | @`

**Syntax Symbols**: `( ) [ ] { } ; , ? ?? : ^ ## #`

## Data Types

- **Integer**: 64-bit signed
- **Float**: high-precision (`long double`)
- **String**: UTF-8 encoded
- **List**: dynamic array, mixed types, nestable

## List

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

## Expressions

**Primary**: literals, variables

**Unary**: `-5`, `!1` (logical NOT)

**Binary**: `10 + 5`, `20 - 8`, `6 * 7`, `10 / 3`

**Parenthesized**: `(10 + 5) * 2`

**String**: `"Hello" + " " + "World"`

## Operators

**Arithmetic**: `+`, `-`, `*`, `/`

**Comparison**: `==`, `!=`, `>`, `<`, `>=`, `<=`

**Logical**: `&` (AND), `|` (OR), `!` (NOT)

**Assignment**: [`=`](./Variables.md)

> **Note on `@`**: `@` is a syntactic marker used only inside function bodies (`{ @ expr }`) to designate the return expression. See [Functions](./Variables.md).

**Precedence** (highest to lowest):
1. `!`, `-` (unary)
2. `*`, `/`
3. `+`, `-`
4. `<`, `<=`, `>`, `>=`
5. `==`, `!=`
6. `&`, `|`
7. `=`

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
