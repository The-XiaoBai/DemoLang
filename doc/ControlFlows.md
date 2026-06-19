# Control Flows

## If Statement

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
- Truthy: non-zero integer, non-zero float, non-empty string, non-empty list, and other non-null types
- Falsy: `0`, `0.0`, `""`, empty list (`[]`)

```
x = 5
?(x > 10) { print("large") } : { print("small") }
# Output: small

y = 15
?(y > 10) { print("A") } ??(y > 5) { print("B") } : { print("C") }
# Output: A
```

## While Loop

DemoLang supports while loops with break and continue statements:

```
# Basic while loop
^(condition) { body }

# Break statement (exits loop)
^(condition) { ## }

# Continue statement (skips to next iteration)
^(condition) { # }
```

**Syntax**:
- `^` starts a while loop
- Condition is evaluated before each iteration
- Loop continues while condition is truthy
- `##` (break) exits the loop immediately
- `#` (continue) skips remaining body and re-evaluates condition

```
# Simple counter
i = 1
^(i <= 5) { print(i); i = i + 1 }
# Output: 1 2 3 4 5

# Break example
j = 1
^(j <= 10) {
    ?(j > 5) { ## }  # Break when j > 5
    print(j); j = j + 1
}
# Output: 1 2 3 4 5

# Continue example
k = 0
^(k < 10) {
    k = k + 1
    ?(k % 2 == 0) { # }  # Skip even numbers
    print(k)
}
# Output: 1 3 5 7 9
```
