# Quick Start

## Table of Contents
- [Installation](#installation)
- [Run DemoLang](#run-demolang)
- [First Steps](#first-steps)
- [Basic Data Types](#basic-data-types)
- [Supported Operations](#supported-operations)
- [Next Steps](#next-steps)

## Installation

```bash
git clone https://github.com/The-XiaoBai/DemoLang.git
cd DemoLang
mkdir build && cd build
cmake ..
cmake --build .
```

## Run DemoLang

```bash
./DemoLang          # Linux/macOS
.\DemoLang.exe      # Windows
```

## First Steps

Start the REPL and try basic expressions:

```
[DemoLang]

> 42 + 8
>>> 50

> x = 10
>>> 10

> x * 2
>>> 20

> "Hello, " + "World!"
>>> Hello, World!
```

## Basic Data Types

- **Integers**: `42`, `-17`
- **Floats**: `3.14`, `-0.5`
- **Strings**: `"Hello"`, `"DemoLang"`

## Supported Operations

- **Arithmetic**: `+`, `-`, `*`, `/`
- **Comparison**: `==`, `!=`, `>`, `<`, `>=`, `<=`
- **Logical**: `&`, `|`, `!`
- **Assignment**: `=`

## Next Steps

- Read [Language Syntax](language-syntax.md) for complete grammar
- Check [Code Examples](code-examples.md) for more patterns
- See [Architecture](architecture.md) for technical details