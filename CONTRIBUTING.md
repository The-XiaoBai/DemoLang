# Contributing to DemoLang

Thank you for your interest in contributing! DemoLang is a C++ based scripting language built for educational and demonstration purposes.

## How to Contribute

### Reporting Issues

- Use [GitHub Issues](https://github.com/The-XiaoBai/DemoLang/issues) to report bugs or suggest features.
- Include steps to reproduce, expected behavior, and your environment (OS, compiler).

### Pull Requests

1. Fork the repository and create a branch from `dev`.
2. Follow the existing code style (4-space indentation, descriptive names).
3. Add or update tests under `tests/` when changing functionality.
4. Ensure all tests pass with `ctest`.
5. Update documentation under `doc/` if your change affects user-facing behavior.
6. Open a PR against the `dev` branch.

### Build & Test

```bash
mkdir build && cd build
cmake .. -DBUILD_TESTS=ON
cmake --build .
ctest
```

### Code Style

- C++20 standard
- Header files in `src/include/`
- Implementation files organized by component (`src/`, `src/lexer/`, `src/parser/`, `src/interpreter/`)
- Use `#pragma once` for header guards

## License

By contributing, you agree that your contributions will be licensed under the MIT License.
