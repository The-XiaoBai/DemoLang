/**
 * @file tests/test_controlflow.cpp
 * @brief Full-chain tests for if/else-if/else and while loops with break/continue.
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

class ControlFlowTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class IfStatement : public ControlFlowTestCase {
public:
    void run() override {
        // True branch
        ASSERT(RUN("?(1){100}") == "100");
        // False with no else returns empty-ish (undefined behavior)
        RUN("?(0){100}");
        // False with else
        ASSERT(RUN("?(0){100}:{200}") == "200");
        // If-else if-else
        ASSERT(RUN("?(1){100}??(0){200}:{300}") == "100");
        ASSERT(RUN("?(0){100}??(1){200}:{300}") == "200");
        ASSERT(RUN("?(0){100}??(0){200}:{300}") == "300");
        // Complex condition
        ASSERT(RUN("?(1 == 1){42}") == "42");
        // Variable condition
        RUN("x = 10");
        ASSERT(RUN("?(x > 5){100}:{200}") == "100");
        ASSERT(RUN("?(x < 5){100}:{200}") == "200");
        // Nested if
        RUN("y = 5");
        ASSERT(RUN("?(y > 0){?(y < 10){1}:{0}}:{-1}") == "1");
        // Multiple else-if
        ASSERT(RUN("?(0){1}??(0){2}??(1){3}:{4}") == "3");
    }
};

class WhileLoop : public ControlFlowTestCase {
public:
    void run() override {
        // Sum 1 to 10
        RUN("sum = 0");
        RUN("i = 1");
        RUN("^(i <= 10){sum = sum + i; i = i + 1}");
        ASSERT(RUN("sum") == "55");

        // False condition, body not executed
        RUN("x = 100");
        RUN("^(0){x = 200}");
        ASSERT(RUN("x") == "100");

        // Complex condition
        RUN("y = 0");
        RUN("^(y * y < 100){y = y + 1}");
        ASSERT(RUN("y") == "10");

        // Single iteration
        RUN("once = 0");
        RUN("^(once < 1){once = once + 1}");
        ASSERT(RUN("once") == "1");
    }
};

class BreakStatement : public ControlFlowTestCase {
public:
    void run() override {
        // Break mid-loop
        RUN("sum = 0");
        RUN("i = 1");
        RUN("^(i <= 10){?(i > 5){##}; sum = sum + i; i = i + 1}");
        ASSERT(RUN("sum") == "15");

        // Break at loop start
        RUN("j = 1");
        RUN("^(j <= 5){##; j = j + 1}");
        ASSERT(RUN("j") == "1");

        // Break at specific condition
        RUN("k = 0");
        RUN("^(k < 10){?(k == 3){##}; k = k + 1}");
        ASSERT(RUN("k") == "3");
    }
};

class ContinueStatement : public ControlFlowTestCase {
public:
    void run() override {
        // Skip numbers <= 3
        RUN("sum = 0");
        RUN("i = 0");
        RUN("^(i < 10){i = i + 1; ?(i <= 3){#}; sum = sum + i}");
        ASSERT(RUN("sum") == "49");

        // Skip numbers <= 7
        RUN("count = 0");
        RUN("m = 0");
        RUN("^(m < 20){m = m + 1; ?(m <= 7){#}; count = count + 1}");
        ASSERT(RUN("count") == "13");
    }
};

class NestedControlFlow : public ControlFlowTestCase {
public:
    void run() override {
        // Nested if inside while
        RUN("count = 0");
        RUN("i = 1");
        RUN("^(i <= 5){?(i <= 3){count = count + 1}; i = i + 1}");
        ASSERT(RUN("count") == "3");

        // Nested while loops
        RUN("outer = 0");
        RUN("inner = 0");
        RUN("^(outer < 3){inner = 0; ^(inner < 2){inner = inner + 1}; outer = outer + 1}");
        ASSERT(RUN("outer") == "3");
        ASSERT(RUN("inner") == "2");
    }
};

class ParserVerification : public ControlFlowTestCase {
public:
    void run() override {
        // If: ?(1){100}
        auto ast = parse("?(1){100}");
        auto ifn = dynamic_cast<IfNode*>(ast.get());
        ASSERT(ifn && ifn->getConditions().size() == 1 && ifn->getBodies().size() == 1);
        ASSERT(ifn->getElseBody() == nullptr);

        // If-else: ?(0){100}:{200}
        ast = parse("?(0){100}:{200}");
        ifn = dynamic_cast<IfNode*>(ast.get());
        ASSERT(ifn && ifn->getElseBody() != nullptr);

        // If-else if-else: ?(0){100}??(1){200}:{300}
        ast = parse("?(0){100}??(1){200}:{300}");
        ifn = dynamic_cast<IfNode*>(ast.get());
        ASSERT(ifn && ifn->getConditions().size() == 2);

        // While: ^(i <= 10){sum = sum + i}
        ast = parse("^(i <= 10){sum = sum + i}");
        auto wn = dynamic_cast<WhileNode*>(ast.get());
        ASSERT(wn && wn->getCondition() != nullptr && wn->getBody() != nullptr);

        // Break: ##
        ast = parse("##");
        auto lc = dynamic_cast<LoopControlNode*>(ast.get());
        ASSERT(lc && lc->isBreak());

        // Continue: #
        ast = parse("#");
        lc = dynamic_cast<LoopControlNode*>(ast.get());
        ASSERT(lc && lc->isContinue());
    }
};

int main() {
    TestRunner runner;
    runner.addTest("ControlFlow: If", std::make_shared<IfStatement>());
    runner.addTest("ControlFlow: While", std::make_shared<WhileLoop>());
    runner.addTest("ControlFlow: Break", std::make_shared<BreakStatement>());
    runner.addTest("ControlFlow: Continue", std::make_shared<ContinueStatement>());
    runner.addTest("ControlFlow: Nested", std::make_shared<NestedControlFlow>());
    runner.addTest("ControlFlow: Parser Verification", std::make_shared<ParserVerification>());
    return runner.runAll();
}

#endif // isTEST
