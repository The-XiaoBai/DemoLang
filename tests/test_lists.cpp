/**
 * @file tests/test_lists.cpp
 * @brief Full-chain tests for list creation, index access, nested lists, and out-of-bounds errors.
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

class ListsTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class ListCreation : public ListsTestCase {
public:
    void run() override {
        ASSERT(RUN("[]") == "[]");
        ASSERT(RUN("[1, 2, 3]") == "[1, 2, 3]");
        ASSERT(RUN("[1.5, 2.5, 3.5]") == "[1.500000, 2.500000, 3.500000]");
        ASSERT(RUN("[\"a\", \"b\", \"c\"]") == "[a, b, c]");
        ASSERT(RUN("[1, \"hello\", 3.14]") == "[1, hello, 3.140000]");
        // Nested list expression
        ASSERT(RUN("[1 + 2, 3 * 4]") == "[3, 12]");
    }
};

class ListAssignment : public ListsTestCase {
public:
    void run() override {
        RUN("nums = [1, 2, 3]");
        ASSERT(RUN("nums") == "[1, 2, 3]");

        // Reassign list variable
        RUN("nums = [4, 5]");
        ASSERT(RUN("nums") == "[4, 5]");

        // Assign list to another variable
        RUN("copy = nums");
        ASSERT(RUN("copy") == "[4, 5]");
    }
};

class IndexAccess : public ListsTestCase {
public:
    void run() override {
        RUN("nums = [1, 2, 3]");
        ASSERT(RUN("nums[0]") == "1");
        ASSERT(RUN("nums[1]") == "2");
        ASSERT(RUN("nums[2]") == "3");

        // Index on literal
        ASSERT(RUN("[10, 20, 30][1]") == "20");

        // Index with variable
        RUN("i = 1");
        ASSERT(RUN("nums[i]") == "2");

        // Index with expression
        ASSERT(RUN("nums[0 + 1]") == "2");
    }
};

class NestedList : public ListsTestCase {
public:
    void run() override {
        RUN("nested = [1, [2, 3], [4, 5]]");
        ASSERT(RUN("nested[0]") == "1");
        ASSERT(RUN("nested[1]") == "[2, 3]");
        ASSERT(RUN("nested[2]") == "[4, 5]");

        // Chained index
        ASSERT(RUN("nested[1][0]") == "2");
        ASSERT(RUN("nested[2][1]") == "5");

        // Deep chained index
        RUN("b = [1, 5, [5, \"d\"]]");
        ASSERT(RUN("b[2][0]") == "5");
        ASSERT(RUN("b[2][1]") == "d");

        // Three-level nesting
        RUN("deep = [1, [[2, 3]]]");
        ASSERT(RUN("deep[1][0][1]") == "3");
    }
};

class OutOfBounds : public ListsTestCase {
public:
    void run() override {
        RUN("nums = [1, 2, 3]");
        std::string result = RUN("nums[10]");
        ASSERT(result.find("Index out of bounds") != std::string::npos);

        // Negative index
        result = RUN("nums[-1]");
        ASSERT(result.find("Index out of bounds") != std::string::npos);

        // Empty list index
        RUN("empty = []");
        result = RUN("empty[0]");
        ASSERT(result.find("Index out of bounds") != std::string::npos);
    }
};

class ParserVerification : public ListsTestCase {
public:
    void run() override {
        // [1, 2, 3]
        auto ast = parse("[1, 2, 3]");
        auto ln = dynamic_cast<ListNode*>(ast.get());
        ASSERT(ln && ln->getElements().size() == 3);

        // []
        ast = parse("[]");
        ln = dynamic_cast<ListNode*>(ast.get());
        ASSERT(ln && ln->getElements().size() == 0);

        // [[1, 2], [3, 4]]
        ast = parse("[[1, 2], [3, 4]]");
        ln = dynamic_cast<ListNode*>(ast.get());
        ASSERT(ln && ln->getElements().size() == 2);

        // nums[0]
        ast = parse("nums[0]");
        auto idx = dynamic_cast<IndexNode*>(ast.get());
        ASSERT(idx);
        ASSERT(dynamic_cast<IdNode*>(idx->getObject())->getName() == "nums");
        ASSERT(dynamic_cast<IntNode*>(idx->getIndex())->getValue() == 0);

        // nested[1][0]
        ast = parse("nested[1][0]");
        idx = dynamic_cast<IndexNode*>(ast.get());
        ASSERT(idx);
        ASSERT(dynamic_cast<IndexNode*>(idx->getObject()));
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Lists: Creation", std::make_shared<ListCreation>());
    runner.addTest("Lists: Assignment", std::make_shared<ListAssignment>());
    runner.addTest("Lists: Index Access", std::make_shared<IndexAccess>());
    runner.addTest("Lists: Nested", std::make_shared<NestedList>());
    runner.addTest("Lists: Out of Bounds", std::make_shared<OutOfBounds>());
    runner.addTest("Lists: Parser Verification", std::make_shared<ParserVerification>());
    return runner.runAll();
}

#endif // isTEST
