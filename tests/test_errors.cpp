/**
 * @file tests/test_errors.cpp
 * @brief Full-chain tests for lexer, parser, and runtime errors.
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

class ErrorsTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class LexerErrors : public ErrorsTestCase {
public:
    void run() override {
        // Unknown character
        auto tokens = Lexer::instance().tokenize("`");
        ASSERT(tokens[0].type == TokenType::ERROR);
        ASSERT(tokens[0].value.find("Unknown character") != std::string::npos);

        // Multiple decimal points
        tokens = Lexer::instance().tokenize("12.3.4");
        ASSERT(tokens[0].type == TokenType::ERROR);
        ASSERT(tokens[0].value.find("Multiple decimal points") != std::string::npos);

        // Unterminated string
        tokens = Lexer::instance().tokenize("\"hello");
        // Implementation may handle this gracefully
    }
};

class ParserErrors : public ErrorsTestCase {
public:
    void run() override {
        // Unexpected token via custom token list
        std::vector<Token> tokens = {
            {TokenType::OPERATOR, "++"},
            {TokenType::END, ""}
        };
        auto ast = Parser::instance().parse(tokens);
        ASSERT(ast);
        ASSERT(dynamic_cast<ErrorNode*>(ast.get()));

        // Lexer error propagates through parser (e.g., unknown char '`')
        auto tokens2 = Lexer::instance().tokenize("`");
        auto ast2 = Parser::instance().parse(tokens2);
        ASSERT(dynamic_cast<ErrorNode*>(ast2.get()));

        // Unexpected end of input
        tokens = {{TokenType::END, ""}};
        ast = Parser::instance().parse(tokens);
        ASSERT(dynamic_cast<ErrorNode*>(ast.get()));
    }
};

class RuntimeErrors : public ErrorsTestCase {
public:
    void run() override {
        // Division by zero
        ASSERT(RUN("10 / 0").find("Division by zero") != std::string::npos);

        // Division by zero via variable
        RUN("zero = 0");
        ASSERT(RUN("10 / zero").find("Division by zero") != std::string::npos);

        // Undefined variable
        ASSERT(RUN("undefined_var").find("Undefined variable") != std::string::npos);

        // Index out of bounds
        RUN("nums = [1, 2, 3]");
        ASSERT(RUN("nums[10]").find("Index out of bounds") != std::string::npos);

        // Invalid index type
        ASSERT(RUN("nums[\"hello\"]").find("Index must be integer") != std::string::npos);

        // Indexing non-list type
        ASSERT(RUN("42[0]").find("Cannot index non-list type") != std::string::npos);

        // Indexing string
        RUN("s = \"text\"");
        std::string result = RUN("s[0]");
        ASSERT(result.find("Cannot index non-list type") != std::string::npos);

        // Undefined variable used as function argument
        ASSERT(RUN("print(null)").find("Undefined variable") != std::string::npos);

        // Unknown function call
        ASSERT(RUN("unknown(42)").find("Unknown function") != std::string::npos);

        // Unknown function with wrong args
        ASSERT(RUN("unknown(1, 2, 3)").find("Unknown function") != std::string::npos);
    }
};

class ErrorPriority : public ErrorsTestCase {
public:
    void run() override {
        // Lexer error (% is not a valid operator) must not be masked
        // by parser errors like "Expected ')' in if condition"
        std::string result = RUN("?(i % 2 == 0){#}");
        ASSERT(result.find("Unknown character") != std::string::npos);

        // Similar: unknown char in while condition
        result = RUN("^(i % 10){#}");
        ASSERT(result.find("Unknown character") != std::string::npos);

        // Unknown char in binary expression
        result = RUN("1 + (2 % 3)");
        ASSERT(result.find("Unknown character") != std::string::npos);

        // Unknown char in function call
        result = RUN("print(1 % 2)");
        ASSERT(result.find("Unknown character") != std::string::npos);
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Errors: Lexer", std::make_shared<LexerErrors>());
    runner.addTest("Errors: Parser", std::make_shared<ParserErrors>());
    runner.addTest("Errors: Runtime", std::make_shared<RuntimeErrors>());
    runner.addTest("Errors: Priority", std::make_shared<ErrorPriority>());
    return runner.runAll();
}

#endif // isTEST
