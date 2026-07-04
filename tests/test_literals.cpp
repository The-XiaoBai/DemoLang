/**
 * @file tests/test_literals.cpp
 * @brief Full-chain tests for integer, float, string, and boolean literals.
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

class LiteralsTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class IntegerLiterals : public LiteralsTestCase {
public:
    void run() override {
        ASSERT(RUN("42") == "42");
        ASSERT(RUN("0") == "0");
        ASSERT(RUN("-10") == "-10");
        ASSERT(RUN("1234567890") == "1234567890");

        // Token level verification
        auto tokens = Lexer::instance().tokenize("123");
        ASSERT(tokens[0].type == TokenType::INTEGER_LITERAL);
        ASSERT(tokens[0].value == "123");

        // AST level verification
        auto ast = parse("123");
        auto i = dynamic_cast<IntNode*>(ast.get());
        ASSERT(i && i->getValue() == 123);

        // Large number
        ast = parse("999999999");
        i = dynamic_cast<IntNode*>(ast.get());
        ASSERT(i && i->getValue() == 999999999);
    }
};

class FloatLiterals : public LiteralsTestCase {
public:
    void run() override {
        ASSERT(RUN("3.14") == "3.140000");
        ASSERT(RUN("0.5") == "0.500000");
        ASSERT(RUN("-2.5") == "-2.500000");

        // Token level
        auto tokens = Lexer::instance().tokenize("45.6");
        ASSERT(tokens[0].type == TokenType::FLOAT_LITERAL);
        ASSERT(tokens[0].value == "45.6");

        // AST level
        auto ast = parse("1.23");
        auto f = dynamic_cast<FloatNode*>(ast.get());
        ASSERT(f && f->getValue() == 1.23);
    }
};

class StringLiterals : public LiteralsTestCase {
public:
    void run() override {
        ASSERT(RUN("\"hello\"") == "hello");
        ASSERT(RUN("\"\"") == "");
        ASSERT(RUN("\"Hello, World!\"") == "Hello, World!");

        // AST level
        auto ast = parse("\"hello\"");
        auto s = dynamic_cast<StringNode*>(ast.get());
        ASSERT(s && s->getValue() == "hello");

        // Empty string AST
        ast = parse("\"\"");
        s = dynamic_cast<StringNode*>(ast.get());
        ASSERT(s && s->getValue() == "");
    }
};

class BooleanLiterals : public LiteralsTestCase {
public:
    void run() override {
        ASSERT(RUN("1") == "1");
        ASSERT(RUN("0") == "0");
        ASSERT(RUN("1 == 1") == "1");
        ASSERT(RUN("1 == 2") == "0");
        ASSERT(RUN("1 < 2") == "1");
        ASSERT(RUN("2 < 1") == "0");
    }
};

class MixedLiteralExpressions : public LiteralsTestCase {
public:
    void run() override {
        ASSERT(RUN("1 + 2 + 3") == "6");
        ASSERT(RUN("2 + 3.5") == "5.500000");
        ASSERT(RUN("\"Hello, \" + \"World!\"") == "Hello, World!");

        // Multiple decimal points error
        std::string result = RUN("12.3.4");
        ASSERT(result.find("Multiple decimal points") != std::string::npos);
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Literals: Integer", std::make_shared<IntegerLiterals>());
    runner.addTest("Literals: Float", std::make_shared<FloatLiterals>());
    runner.addTest("Literals: String", std::make_shared<StringLiterals>());
    runner.addTest("Literals: Boolean", std::make_shared<BooleanLiterals>());
    runner.addTest("Literals: Mixed Expressions", std::make_shared<MixedLiteralExpressions>());
    return runner.runAll();
}

#endif // isTEST
