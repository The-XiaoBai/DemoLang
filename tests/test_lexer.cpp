#ifdef isTEST

#include "test_framework.hpp"
#include "tokens.hpp"
#include "lexer.hpp"

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;


class LexerTestCase : public TestCase {
protected:
    Lexer* lexer;
    void setUp() override { lexer = &Lexer::instance(); }
    void tearDown() override { lexer = nullptr; }
};


class TestOperators : public LexerTestCase {
public:
    void run() override {
        auto tokens = lexer->tokenize("+-*/ != <= < =");

        auto token1 = tokens[0];
        auto token2 = tokens[1];
        auto token3 = tokens[2];
        auto token4 = tokens[3];
        auto token5 = tokens[4];
        auto token6 = tokens[5];
        auto token7 = tokens[6];
        auto token8 = tokens[7];
        auto token9 = tokens[8];

        assert(token1.type == TokenType::OPERATOR);
        assert(token1.value == "+");
        assert(token2.type == TokenType::OPERATOR);
        assert(token2.value == "-");
        assert(token3.type == TokenType::OPERATOR);
        assert(token3.value == "*");
        assert(token4.type == TokenType::OPERATOR);
        assert(token4.value == "/");
        assert(token5.type == TokenType::OPERATOR);
        assert(token5.value == "!=");
        assert(token6.type == TokenType::OPERATOR);
        assert(token6.value == "<=");
        assert(token7.type == TokenType::OPERATOR);
        assert(token7.value == "<");
        assert(token8.type == TokenType::OPERATOR);
        assert(token8.value == "=");
        assert(token9.type == TokenType::END);
    }
};


class TestIdentifiers : public LexerTestCase {
public:
    void run() override {
        auto tokens = lexer->tokenize("foo bar_123 _1a");

        auto token1 = tokens[0];
        auto token2 = tokens[1];
        auto token3 = tokens[2];
        auto token4 = tokens[3];

        assert(token1.type == TokenType::IDENTIFIER);
        assert(token1.value == "foo");
        assert(token2.type == TokenType::IDENTIFIER);
        assert(token2.value == "bar_123");
        assert(token3.type == TokenType::IDENTIFIER);
        assert(token3.value == "_1a");
        assert(token4.type == TokenType::END);
    }
};


class TestLiterals : public LexerTestCase {
public:
    void run() override {
        auto tokens = lexer->tokenize("123 45.6 \"abc\" 12.3.4");
        
        auto token1 = tokens[0];
        auto token2 = tokens[1];
        auto token3 = tokens[2];
        auto token4 = tokens[3];
        auto token5 = tokens[4];
        
        assert(token1.type == TokenType::INTEGER_LITERAL);
        assert(token1.value == "123");
        assert(token2.type == TokenType::FLOAT_LITERAL);
        assert(token2.value == "45.6");
        assert(token3.type == TokenType::STRING_LITERAL);
        assert(token3.value == "\"abc\"");
        assert(token4.type == TokenType::ERROR);
        assert(token4.value == "Multiple decimal points");
        assert(token5.type == TokenType::END);
    }
};


class TestErrors : public LexerTestCase {
public:
    void run() override {
        auto tokens = lexer->tokenize("@#");

        auto token1 = tokens[0];
        auto token2 = tokens[1];

        assert(token1.type == TokenType::ERROR);
        assert(token1.value == "Unknown character: " + std::string(1, '@'));
        assert(token2.type == TokenType::END);
    }
};


class TestEmptyInput : public LexerTestCase {
public:
    void run() override {
        auto tokens = lexer->tokenize("");

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::END);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Lexer: Operators", std::make_shared<TestOperators>());
    runner.addTest("Lexer: Identifiers", std::make_shared<TestIdentifiers>());
    runner.addTest("Lexer: Literals", std::make_shared<TestLiterals>());
    runner.addTest("Lexer: Errors", std::make_shared<TestErrors>());
    runner.addTest("Lexer: Empty Input", std::make_shared<TestEmptyInput>());
    runner.runAll();

    return 0;
}

#endif // isTEST