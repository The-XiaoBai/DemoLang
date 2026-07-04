/**
 * @file tests/test_operators.cpp
 * @brief Full-chain tests for arithmetic, comparison, logical, unary operators and string operations.
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

/** Helper: tokenize + parse in one step. */
static std::shared_ptr<ASTNode> parse(const std::string& src) {
    auto tokens = Lexer::instance().tokenize(src);
    return Parser::instance().parse(tokens);
}

class OperatorsTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class BasicOperators : public OperatorsTestCase {
public:
    void run() override {
        ASSERT(RUN("1 + 2") == "3");
        ASSERT(RUN("5 - 3") == "2");
        ASSERT(RUN("4 * 6") == "24");
        ASSERT(RUN("10 / 2") == "5.000000");
        ASSERT(RUN("2 + 3 * 4") == "14");
        ASSERT(RUN("(2 + 3) * 4") == "20");
        ASSERT(RUN("1 + 2 - 3") == "0");
        ASSERT(RUN("2 * 3 + 4") == "10");
        ASSERT(RUN("10 / 2 * 3") == "15.000000");
    }
};

class FloatArithmetic : public OperatorsTestCase {
public:
    void run() override {
        ASSERT(RUN("1.5 + 2.5") == "4.000000");
        ASSERT(RUN("2.0 * 3.5") == "7.000000");
        ASSERT(RUN("2 + 3.5") == "5.500000");
        ASSERT(RUN("3.14 - 1.14") == "2.000000");
    }
};

class ComparisonOperators : public OperatorsTestCase {
public:
    void run() override {
        ASSERT(RUN("1 == 1") == "1");
        ASSERT(RUN("1 == 2") == "0");
        ASSERT(RUN("1 != 2") == "1");
        ASSERT(RUN("1 != 1") == "0");
        ASSERT(RUN("1 < 2") == "1");
        ASSERT(RUN("2 < 1") == "0");
        ASSERT(RUN("1 <= 1") == "1");
        ASSERT(RUN("1 <= 2") == "1");
        ASSERT(RUN("2 <= 1") == "0");
        ASSERT(RUN("2 > 1") == "1");
        ASSERT(RUN("1 > 2") == "0");
        ASSERT(RUN("2 >= 1") == "1");
        ASSERT(RUN("2 >= 2") == "1");
        ASSERT(RUN("1 >= 2") == "0");
        // Float comparisons
        ASSERT(RUN("1.5 == 1.5") == "1");
        ASSERT(RUN("1.5 != 1.5") == "0");
        // String comparisons
        ASSERT(RUN("\"a\" == \"a\"") == "1");
        ASSERT(RUN("\"a\" == \"b\"") == "0");
        ASSERT(RUN("\"a\" != \"b\"") == "1");
    }
};

class LogicalOperators : public OperatorsTestCase {
public:
    void run() override {
        ASSERT(RUN("1 & 1") == "1");
        ASSERT(RUN("1 & 0") == "0");
        ASSERT(RUN("0 & 1") == "0");
        ASSERT(RUN("0 & 0") == "0");
        ASSERT(RUN("1 | 1") == "1");
        ASSERT(RUN("1 | 0") == "1");
        ASSERT(RUN("0 | 1") == "1");
        ASSERT(RUN("0 | 0") == "0");
        ASSERT(RUN("!0") == "1");
        ASSERT(RUN("!1") == "0");
        // Complex logical expressions
        ASSERT(RUN("!(1 == 1)") == "0");
        ASSERT(RUN("(1 & 1) | 0") == "1");
        // Operator precedence: comparison before logical
        ASSERT(RUN("1 < 2 & 2 < 3") == "1");
    }
};

class StringOperations : public OperatorsTestCase {
public:
    void run() override {
        // Concatenation
        ASSERT(RUN("\"Hello, \" + \"World!\"") == "Hello, World!");
        ASSERT(RUN("\"\" + \"test\"") == "test");
        // With variables
        RUN("a = \"hello\"");
        RUN("b = \" world\"");
        ASSERT(RUN("a + b") == "hello world");
    }
};

class UnaryOperators : public OperatorsTestCase {
public:
    void run() override {
        ASSERT(RUN("-5") == "-5");
        ASSERT(RUN("-(3 + 4)") == "-7");
        ASSERT(RUN("!0") == "1");
        ASSERT(RUN("!1") == "0");
        // Negation in expressions
        ASSERT(RUN("10 + (-3)") == "7");
        ASSERT(RUN("(-2) * 5") == "-10");
    }
};

class ParserVerification : public OperatorsTestCase {
public:
    void run() override {
        // Unary op: -5
        auto ast = parse("-5");
        auto u = dynamic_cast<UnaryOpNode*>(ast.get());
        ASSERT(u && u->getOp() == "-");
        ASSERT(dynamic_cast<IntNode*>(u->getOperand())->getValue() == 5);

        // Unary op: !0
        ast = parse("!0");
        u = dynamic_cast<UnaryOpNode*>(ast.get());
        ASSERT(u && u->getOp() == "!");

        // Binary op: a = 1
        ast = parse("a = 1");
        auto b = dynamic_cast<BinaryOpNode*>(ast.get());
        ASSERT(b && b->getOp() == "=");
        ASSERT(dynamic_cast<IdNode*>(b->getLeft())->getName() == "a");

        // Precedence: (2 + 3) * 4
        ast = parse("(2 + 3) * 4");
        b = dynamic_cast<BinaryOpNode*>(ast.get());
        ASSERT(b && b->getOp() == "*");

        // Precedence: 1 + 2 * 3
        ast = parse("1 + 2 * 3");
        b = dynamic_cast<BinaryOpNode*>(ast.get());
        ASSERT(b && b->getOp() == "+");
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Operators: Basic", std::make_shared<BasicOperators>());
    runner.addTest("Operators: Float", std::make_shared<FloatArithmetic>());
    runner.addTest("Operators: Comparison", std::make_shared<ComparisonOperators>());
    runner.addTest("Operators: Logical", std::make_shared<LogicalOperators>());
    runner.addTest("Operators: String", std::make_shared<StringOperations>());
    runner.addTest("Operators: Unary", std::make_shared<UnaryOperators>());
    runner.addTest("Operators: Parser Verification", std::make_shared<ParserVerification>());
    return runner.runAll();
}

#endif // isTEST
