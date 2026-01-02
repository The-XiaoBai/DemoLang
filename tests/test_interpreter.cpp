#ifdef isTEST

#include "test_framework.hpp"
#include "ast.hpp"
#include "builtins.hpp"
#include "interpreter.hpp"

using namespace DemoLang;
using namespace DemoLang::AST;
using namespace DemoLang::ValueTypes;
using namespace DemoLang::InterpreterSpace;


class InterpreterTestCase : public TestCase {
protected:
    Interpreter* interpreter;
    void setUp() override { interpreter = &Interpreter::instance(); }
    void tearDown() override { interpreter = nullptr; }
};


class TestUnaryOperators : public InterpreterTestCase {
public:
    void run() override {
        // Test unary minus
        auto operand = std::make_shared<IntNode>(5);
        auto unaryMinus = std::make_shared<UnaryOpNode>("-", operand);
        std::string result = interpreter->interpret(unaryMinus);
        assert(result == "-5");

        // Test unary not
        auto zero = std::make_shared<IntNode>(0);
        auto unaryNot = std::make_shared<UnaryOpNode>("!", zero);
        result = interpreter->interpret(unaryNot);
        assert(result == "1");
    }
};


class TestBinaryOperators : public InterpreterTestCase {
public:
    void run() override {
        // Test string addition
        auto left_str = std::make_shared<StringNode>("Hello, ");
        auto right_str = std::make_shared<StringNode>("World!");
        auto addNode_str = std::make_shared<BinaryOpNode>("+", left_str, right_str);
        std::string result = interpreter->interpret(addNode_str);
        assert(result == "Hello, World!");

        // Test numeric addition
        auto left = std::make_shared<IntNode>(2);
        auto right = std::make_shared<IntNode>(3);
        auto addNode = std::make_shared<BinaryOpNode>("+", left, right);
        result = interpreter->interpret(addNode);
        assert(result == "5");

        // Test multiplication
        auto mulNode = std::make_shared<BinaryOpNode>("*", left, right);
        result = interpreter->interpret(mulNode);
        assert(result == "6");

        // Test assignment
        auto idNode = std::make_shared<IdNode>("x");
        auto valueNode = std::make_shared<IntNode>(10);
        auto assignNode = std::make_shared<BinaryOpNode>("=", idNode, valueNode);
        result = interpreter->interpret(assignNode);
        assert(result == "10");
    }
};


class TestLiterals : public InterpreterTestCase {
public:
    void run() override {
        // Test integer literal
        auto intNode = std::make_shared<IntNode>(42);
        std::string result = interpreter->interpret(intNode);
        assert(result == "42");

        // Test float literal
        auto floatNode = std::make_shared<FloatNode>(3.14);
        result = interpreter->interpret(floatNode);
        assert(result == "3.140000");

        // Test string literal
        auto strNode = std::make_shared<StringNode>("hello");
        result = interpreter->interpret(strNode);
        assert(result == "hello");
    }
};


class TestVariables : public InterpreterTestCase {
public:
    void run() override {
        // Test assignment
        auto idNode = std::make_shared<IdNode>("y");
        auto valueNode = std::make_shared<IntNode>(20);
        auto assignNode = std::make_shared<BinaryOpNode>("=", idNode, valueNode);
        std::string result = interpreter->interpret(assignNode);
        assert(result == "20");

        // Test variable lookup
        result = interpreter->interpret(idNode);
        assert(result == "20");
    }
};


class TestErrorHandling : public InterpreterTestCase {
public:
    void run() override {
        // Test error node
        auto errorNode = std::make_shared<ErrorNode>("Error");
        std::string result = interpreter->interpret(errorNode);
        assert(result == "Error");

        // Test null node
        result = interpreter->interpret(nullptr);
        assert(result == "Null AST Node");
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Interpreter: Unary Operators", std::make_shared<TestUnaryOperators>());
    runner.addTest("Interpreter: Binary Operators", std::make_shared<TestBinaryOperators>());
    runner.addTest("Interpreter: Literals", std::make_shared<TestLiterals>());
    runner.addTest("Interpreter: Variables", std::make_shared<TestVariables>());
    runner.addTest("Interpreter: Error Handling", std::make_shared<TestErrorHandling>());
    runner.runAll();

    return 0;
}

#endif // isTEST