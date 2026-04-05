/**
 * @file tests/test_interpreter.cpp
 * @brief Unit tests for the interpreter module.
 **/

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


class TestBuiltinFunctions : public InterpreterTestCase {
public:
    void run() override {
        std::string result;

        // Test print with integer
        auto intArg = std::make_shared<IntNode>(42);
        auto printCall1 = std::make_shared<FunctionCallNode>("print", std::vector<std::shared_ptr<ASTNode>>{intArg});
        result = interpreter->interpret(printCall1);
        assert(result == "");

        // Test print with float
        auto floatArg = std::make_shared<FloatNode>(3.14);
        auto printCall2 = std::make_shared<FunctionCallNode>("print", std::vector<std::shared_ptr<ASTNode>>{floatArg});
        result = interpreter->interpret(printCall2);
        assert(result == "");

        // Test print with string
        auto strArg = std::make_shared<StringNode>("hello");
        auto printCall3 = std::make_shared<FunctionCallNode>("print", std::vector<std::shared_ptr<ASTNode>>{strArg});
        result = interpreter->interpret(printCall3);
        assert(result == "");

        // Test print with multiple arguments
        auto intArg2 = std::make_shared<IntNode>(1);
        auto floatArg2 = std::make_shared<FloatNode>(2.5);
        auto strArg2 = std::make_shared<StringNode>("PrintFunctionTests");
        auto printCall4 = std::make_shared<FunctionCallNode>("print", std::vector<std::shared_ptr<ASTNode>>{intArg2, floatArg2, strArg2});
        result = interpreter->interpret(printCall4);
        assert(result == "");

        // Test exit with no arguments (default exit code 0)
        auto exitCall1 = std::make_shared<FunctionCallNode>("exit", std::vector<std::shared_ptr<ASTNode>>{});
        result = interpreter->interpret(exitCall1);
        assert(result == "");

        // Test exit with integer argument
        auto exitCode = std::make_shared<IntNode>(1);
        auto exitCall2 = std::make_shared<FunctionCallNode>("exit", std::vector<std::shared_ptr<ASTNode>>{exitCode});
        result = interpreter->interpret(exitCall2);
        assert(result == "");

        // Test unknown function
        auto unknownCall = std::make_shared<FunctionCallNode>("unknown", std::vector<std::shared_ptr<ASTNode>>{intArg});
        result = interpreter->interpret(unknownCall);
        assert(result == "Unknown function: unknown");
    }
};


class TestIfStatement : public InterpreterTestCase {
public:
    void run() override {
        // If true branch
        auto ifTrue = std::make_shared<IfNode>(
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(1)},
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(100)},
            nullptr
        );
        std::string result = interpreter->interpret(ifTrue);
        assert(result == "100");

        // If false with else
        auto ifElse = std::make_shared<IfNode>(
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(0)},
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(100)},
            std::make_shared<IntNode>(200)
        );
        result = interpreter->interpret(ifElse);
        assert(result == "200");

        // Else-if: first false, second true
        auto ifElseIf = std::make_shared<IfNode>(
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(0), std::make_shared<IntNode>(1)},
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(100), std::make_shared<IntNode>(200)},
            std::make_shared<IntNode>(300)
        );
        result = interpreter->interpret(ifElseIf);
        assert(result == "200");

        // All false, fall to else
        auto allFalse = std::make_shared<IfNode>(
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(0), std::make_shared<IntNode>(0)},
            std::vector<std::shared_ptr<ASTNode>>{std::make_shared<IntNode>(100), std::make_shared<IntNode>(200)},
            std::make_shared<IntNode>(300)
        );
        result = interpreter->interpret(allFalse);
        assert(result == "300");
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Interpreter: Unary Operators", std::make_shared<TestUnaryOperators>());
    runner.addTest("Interpreter: Binary Operators", std::make_shared<TestBinaryOperators>());
    runner.addTest("Interpreter: Literals", std::make_shared<TestLiterals>());
    runner.addTest("Interpreter: Variables", std::make_shared<TestVariables>());
    runner.addTest("Interpreter: Error Handling", std::make_shared<TestErrorHandling>());
    runner.addTest("Interpreter: Built-in Functions", std::make_shared<TestBuiltinFunctions>());
    runner.addTest("Interpreter: If Statement", std::make_shared<TestIfStatement>());
    runner.runAll();

    return 0;
}

#endif // isTEST