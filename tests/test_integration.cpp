/**
 * @file tests/test_integration.cpp
 * @brief Full-chain integration tests combining multiple language features.
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

class IntegrationTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class Fibonacci : public IntegrationTestCase {
public:
    void run() override {
        // Iterative fibonacci(10) = 55
        RUN("fib = (n) { a = 0; b = 1; i = 0; ^(i < n) { temp = a + b; a = b; b = temp; i = i + 1 }; @ a }");
        ASSERT(RUN("fib(10)") == "55");
        ASSERT(RUN("fib(1)") == "1");
        ASSERT(RUN("fib(0)") == "0");
    }
};

class ListTraversal : public IntegrationTestCase {
public:
    void run() override {
        RUN("nums = [1, 2, 3, 4, 5]");
        RUN("sum = 0");
        RUN("i = 0");
        RUN("^(i < 5) { sum = sum + nums[i]; i = i + 1 }");
        ASSERT(RUN("sum") == "15");
    }
};

class FunctionComposition : public IntegrationTestCase {
public:
    void run() override {
        RUN("double = (x) { @ x * 2 }");
        RUN("add_one = (x) { @ x + 1 }");
        ASSERT(RUN("double(add_one(5))") == "12");
        ASSERT(RUN("add_one(double(5))") == "11");
        ASSERT(RUN("add_one(double(add_one(0)))") == "3");
    }
};

class ComplexControlFlow : public IntegrationTestCase {
public:
    void run() override {
        // Sum of numbers <= 5 from 1..10
        RUN("sum = 0");
        RUN("i = 1");
        RUN("^(i <= 10) { ?(i <= 5) { sum = sum + i }; i = i + 1 }");
        ASSERT(RUN("sum") == "15");
    }
};

class StringOperations : public IntegrationTestCase {
public:
    void run() override {
        RUN("first = \"Hello\"");
        RUN("last = \"World\"");
        RUN("greeting = first + \", \" + last + \"!\"");
        ASSERT(RUN("greeting") == "Hello, World!");
    }
};

class MixedTypeList : public IntegrationTestCase {
public:
    void run() override {
        RUN("mixed = [1, \"hello\", 3.14, [2, 3]]");
        ASSERT(RUN("mixed[0]") == "1");
        ASSERT(RUN("mixed[1]") == "hello");
        ASSERT(RUN("mixed[2]") == "3.140000");
        ASSERT(RUN("mixed[3]") == "[2, 3]");
    }
};

class LoopWithEarlyExit : public IntegrationTestCase {
public:
    void run() override {
        // Count to 5 with early exit using break
        RUN("counter = 0");
        RUN("^(counter < 10) { counter = counter + 1; ?(counter >= 5) { ## } }");
        ASSERT(RUN("counter") == "5");
    }
};

class LoopWithContinueAndBreak : public IntegrationTestCase {
public:
    void run() override {
        // Sum even numbers under 10, break at 9
        RUN("sum = 0");
        RUN("i = 0");
        RUN("^(i < 20) { i = i + 1; ?(i >= 10){##}; ?(i == 1 | i == 3 | i == 5 | i == 7 | i == 9){#}; sum = sum + i }");
        // Sum of 2,4,6,8 = 20
        ASSERT(RUN("sum") == "20");
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Integration: Fibonacci", std::make_shared<Fibonacci>());
    runner.addTest("Integration: List Traversal", std::make_shared<ListTraversal>());
    runner.addTest("Integration: Function Composition", std::make_shared<FunctionComposition>());
    runner.addTest("Integration: Complex Control Flow", std::make_shared<ComplexControlFlow>());
    runner.addTest("Integration: String Operations", std::make_shared<StringOperations>());
    runner.addTest("Integration: Mixed Type List", std::make_shared<MixedTypeList>());
    runner.addTest("Integration: Loop With Early Exit", std::make_shared<LoopWithEarlyExit>());
    runner.addTest("Integration: Loop With Continue & Break", std::make_shared<LoopWithContinueAndBreak>());
    return runner.runAll();
}

#endif // isTEST
