# Variables and Functions

## Variables

Dynamically typed — type inferred on first assignment:

```
x = 42           # Integer
name = "Demo"    # String
pi = 3.14        # Float

x = 3.14         # Reassign to different type
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

## User-Defined Functions

Functions are defined using lambda syntax:

### Function Definition

Assign a lambda to a variable:

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

Lambda functions can be:
1. Assigned to variables
2. Immediately invoked
3. Passed as values (first-class)

```
# Lambda assigned to a variable
square = (x) { @ x * x }
result = square(5)           # result = 25

# Lambda with immediate execution
result = (a, b) { @ a + b }(10, 20)  # Returns 30
```

### Function Calls

Called with positional arguments:

```
# Call without arguments
greet()                      # Returns: Hello, World!

# Call with arguments
add(10, 20)                  # Returns 30

# Override default parameter
greet("Alice")               # Returns: Hello, Alice!
```

### Default Parameters

Parameters can have defaults:

```
# Function with default parameter
calc = (a = 10, b = 20) { @ a + b }

# Call with defaults
calc()                       # Returns 30

# Override defaults
calc(5, 3)                   # Returns 8
```

## Built-in Functions

Built-in functions:

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
