# Getting Started

## DemoLang Overview

DemoLang is a simple interpreted scripting language.

### Hello World

```
[DemoLang]

> 1000 + 24
>>> 1024

> name = "DemoLang"
>>> DemoLang

> "Hi, " + name + "!"
>>> Hi, DemoLang!
```

## Two Ways to Run

### REPL (Shell)

Type and execute code line by line:

```bash
./Shell          # Linux/macOS
.\Shell.exe      # Windows
```

### File Execution (FileLoader)

Save code to a file (e.g. `demo`) and run it:

```
x = 10
y = 20
result = x + y
name = "DemoLang"
greeting = "Hello, " + name
print(greeting)
```

Then execute:

```bash
./FileLoader demo           # Linux/macOS
.\FileLoader.exe demo       # Windows
```

Output: `Hello, DemoLang`

## Semicolon `;` Syntax

The semicolon (`;`) separates multiple statements on one line or within a block:

```
# Multiple statements on one line with semicolon
x = 10; y = 20; print(x + y)  # Output: 30

# Statements in control flow blocks
counter = 0
^(counter < 3) {
    print(counter);
    counter = counter + 1
}
# Output: 0 1 2

# Complex example with mixed statements
a = 5; b = 10
^(a < b) {
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
