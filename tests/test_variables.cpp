/**
 * @file tests/test_variables.cpp
 * @brief Full-chain tests for variable assignment, lookup, reassignment, and scope.
 **/

#ifdef isTEST

#include "test_framework.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "ast.hpp"

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;
using namespace DemoLang::AST;
using namespace DemoLang::ParserSpace;
using namespace DemoLang::InterpreterSpace;

#define RUN(src) [&]() -> std::string { \
    auto tokens = Lexer::instance().tokenize(src); \
    auto ast = Parser::instance().parse(tokens); \
    return Interpreter::instance().interpret(ast); \
}()

class VariablesTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class VariableAssignment : public VariablesTestCase {
public:
    void run() override {
        ASSERT(RUN("x = 10") == "10");
        ASSERT(RUN("y = 2 + 3") == "5");
        ASSERT(RUN("name = \"DemoLang\"") == "DemoLang");
        ASSERT(RUN("pi = 3.14") == "3.140000");
        ASSERT(RUN("z = -5") == "-5");
        // Chained expressions
        ASSERT(RUN("a = 1 + 2 * 3") == "7");
    }
};

class VariableLookup : public VariablesTestCase {
public:
    void run() override {
        RUN("x = 42");
        ASSERT(RUN("x") == "42");
        RUN("y = 10");
        ASSERT(RUN("x + y") == "52");
        RUN("a = 1");
        RUN("b = 2");
        RUN("c = 3");
        ASSERT(RUN("a + b + c") == "6");
    }
};

class VariableReassignment : public VariablesTestCase {
public:
    void run() override {
        RUN("x = 10");
        ASSERT(RUN("x") == "10");
        RUN("x = 20");
        ASSERT(RUN("x") == "20");
        RUN("y = 5");
        RUN("y = y + 1");
        ASSERT(RUN("y") == "6");
        // Type change on reassignment
        RUN("z = 10");
        RUN("z = \"hello\"");
        ASSERT(RUN("z") == "hello");
        // Float to int
        RUN("n = 3.14");
        ASSERT(RUN("n") == "3.140000");
        RUN("n = 0");
        ASSERT(RUN("n") == "0");
    }
};

class UndefinedVariable : public VariablesTestCase {
public:
    void run() override {
        ASSERT(RUN("undefined_var").find("Undefined variable") != std::string::npos);
        ASSERT(RUN("x + 1").find("Undefined variable") != std::string::npos);
    }
};

class MultipleStatements : public VariablesTestCase {
public:
    void run() override {
        RUN("x = 10; y = 20; z = 30");
        ASSERT(RUN("x + y + z") == "60");
        // Semicolons with expressions
        RUN("a = 1; b = a + 1");
        ASSERT(RUN("b") == "2");
        // Trailing semicolon
        RUN("c = 100;");
        ASSERT(RUN("c") == "100");
        // Multi-statement with last value being the result
        ASSERT(RUN("1; 2; 3") == "3");
    }
};

class VariableScope : public VariablesTestCase {
public:
    void run() override {
        // Local variable in function
        RUN("f = () { x = 10; @ x }");
        ASSERT(RUN("f()") == "10");

        // Function parameter
        RUN("f = (x) { @(x + 1) }");
        ASSERT(RUN("f(5)") == "6");

        // Variable shadowing
        RUN("x = 100");
        RUN("f = (x) { @(x * 2) }");
        ASSERT(RUN("f(10)") == "20");
        ASSERT(RUN("x") == "100");  // Global x unchanged

        // Access global variable inside function
        RUN("global_var = 42");
        RUN("g = () { @global_var }");
        ASSERT(RUN("g()") == "42");

        // Function modifying global variable
        RUN("counter = 0");
        RUN("inc = () { counter = counter + 1 }");
        RUN("inc()");
        ASSERT(RUN("counter") == "1");
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Variables: Assignment", std::make_shared<VariableAssignment>());
    runner.addTest("Variables: Lookup", std::make_shared<VariableLookup>());
    runner.addTest("Variables: Reassignment", std::make_shared<VariableReassignment>());
    runner.addTest("Variables: Undefined", std::make_shared<UndefinedVariable>());
    runner.addTest("Variables: Multiple Statements", std::make_shared<MultipleStatements>());
    runner.addTest("Variables: Scope", std::make_shared<VariableScope>());
    return runner.runAll();
}

#endif // isTEST
