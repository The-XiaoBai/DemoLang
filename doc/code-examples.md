# Code Examples

## Table of Contents
- [Arithmetic Operations](#arithmetic-operations)
- [Variable Operations](#variable-operations)
- [Comparison Operations](#comparison-operations)
- [Logical Operations](#logical-operations)
- [Mixed Type Operations](#mixed-type-operations)
- [Error Handling](#error-handling)

## Arithmetic Operations

### Basic Math
```
> 10 + 5
>>> 15

> 20 - 8
>>> 12

> 6 * 7
>>> 42
```

### Operator Precedence
```
> 10 + 5 * 2
>>> 20

> (10 + 5) * 2
>>> 30

> 100 - 20 / 5
>>> 96
```

### Float Operations
```
> 3.14 * 2
>>> 6.280000

> 10.0 / 3
>>> 3.333333

> 4.5 + 2.3
>>> 6.800000
```

## Variable Operations

### Assignment and Usage
```
> x = 10
>>> 10

> y = x * 2
>>> 20

> x + y
>>> 30
```

### String Variables
```
> name = "DemoLang"
>>> DemoLang

> greeting = "Hello, " + name + "!"
>>> Hello, DemoLang!
```

## Comparison Operations

### Numeric Comparisons
```
> 10 > 5
>>> 1

> 3.14 < 2
>>> 0

> 42 == 42
>>> 1

> 10 != 20
>>> 1
```

## Logical Operations

### AND/OR Operations
```
> 1 & 1
>>> 1

> 1 & 0
>>> 0

> 0 | 1
>>> 1

> !1
>>> 0
```

### Complex Expressions
```
> (10 > 5) & (3 < 8)
>>> 1

> (x = 5) > 3 | (y = 2) > 10
>>> 1
```

## Mixed Type Operations

### Type Coercion
```
> 10 + 3.14
>>> 13.140000
```

## Error Handling

### Division by Zero
```
> 10 / 0
>>> Division by zero
```

### Type Errors
```
> "hello" * 5
>>> Operands must be numeric
```