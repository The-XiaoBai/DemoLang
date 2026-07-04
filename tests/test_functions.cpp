/**
 * @file tests/test_functions.cpp
 * @brief Full-chain tests for function definition, call, lambda, recursion, and built-in functions.
 **/

#ifdef isTEST

#include "test_framework.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "ast.hpp"
#include <sstream>

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

class FunctionsTestCase : public TestCase {
protected:
    void setUp() override { Interpreter::instance().clearEnvironment(); }
    void tearDown() override {}
};

class FunctionDefinition : public FunctionsTestCase {
public:
    void run() override {
        // No parameters
        RUN("greet = () { @ \"Hello, World!\" }");
        ASSERT(RUN("greet()") == "Hello, World!");

        // With parameters
        RUN("add = (a, b) { @ a + b }");
        ASSERT(RUN("add(10, 20)") == "30");

        // Default parameter
        RUN("greet = (name = \"World\") { @ \"Hello, \" + name + \"!\" }");
        ASSERT(RUN("greet()") == "Hello, World!");
        ASSERT(RUN("greet(\"Alice\")") == "Hello, Alice!");

        // Multiple default params, partial override
        RUN("f = (a = 10, b = 20) { @ a + b }");
        ASSERT(RUN("f()") == "30");
        ASSERT(RUN("f(5)") == "25");

        // No explicit return (returns empty string)
        std::stringstream buffer;
        std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
        RUN("noReturn = () { print(\"hello\") }");
        ASSERT(RUN("noReturn()") == "");
        std::cout.rdbuf(oldCout);

        // Function variable evaluates to [function]
        RUN("f = () {}");
        ASSERT(RUN("f") == "[function]");

        // Multi-statement body with return
        RUN("mult = (x) { y = x * 2; @ y }");
        ASSERT(RUN("mult(7)") == "14");
    }
};

class FunctionCall : public FunctionsTestCase {
public:
    void run() override {
        RUN("square = (x) { @ x * x }");
        ASSERT(RUN("square(5)") == "25");

        // Expression arguments
        RUN("add = (a, b) { @ a + b }");
        ASSERT(RUN("add(2 + 3, 4 * 5)") == "25");

        // Nested calls
        RUN("double_fn = (x) { @ x * 2 }");
        ASSERT(RUN("double_fn(double_fn(5))") == "20");

        // Empty arg list
        RUN("zero = () { @ 0 }");
        ASSERT(RUN("zero()") == "0");
    }
};

class LambdaExpression : public FunctionsTestCase {
public:
    void run() override {
        // Lambda assigned to variable
        RUN("f = (x) { @ x * 2 }");
        ASSERT(RUN("f(10)") == "20");

        // Immediate invocation
        ASSERT(RUN("(a, b) { @ a + b }(10, 20)") == "30");

        // Zero-arg lambda immediate
        ASSERT(RUN("(){ @ 42 }()") == "42");
    }
};

class FunctionScope : public FunctionsTestCase {
public:
    void run() override {
        // Access outer variable
        RUN("x = 10");
        RUN("f = () { @ x }");
        ASSERT(RUN("f()") == "10");

        // Parameter shadows global
        RUN("y = 100");
        RUN("g = (y) { @ y * 2 }");
        ASSERT(RUN("g(3)") == "6");
        ASSERT(RUN("y") == "100");
    }
};

class BuiltinFunctions : public FunctionsTestCase {
public:
    void run() override {
        // === print ===
        // Tesr print function with various argument
        std::stringstream buffer;
        std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

        ASSERT(RUN("print(42)") == "");
        ASSERT(RUN("print(3.14)") == "");
        ASSERT(RUN("print(\"hello\")") == "");
        ASSERT(RUN("print(1, 2.5, \"test\")") == "");

        // Assign print result to variable
        RUN("x = print(10)");
        ASSERT(RUN("x") == "");

        std::cout.rdbuf(oldCout);  // Restore

        // === exit / query ===
        // Verify they are recognized as built-in functions (parse correctly)
        auto ast = parse("exit()");
        auto fc = dynamic_cast<FunctionCallNode*>(ast.get());
        ASSERT(fc && fc->getName() == "exit");

        ast = parse("query()");
        fc = dynamic_cast<FunctionCallNode*>(ast.get());
        ASSERT(fc && fc->getName() == "query");
    }
};

class UnknownFunction : public FunctionsTestCase {
public:
    void run() override {
        ASSERT(RUN("unknown()").find("Unknown function") != std::string::npos);
        ASSERT(RUN("unknown(1, 2, 3)").find("Unknown function") != std::string::npos);

        // Wrong number of arguments
        RUN("f = (x) { @ x }");
        std::string result = RUN("f()");
        ASSERT(!result.empty());  // Should produce an error
    }
};

class ParserVerification : public FunctionsTestCase {
public:
    void run() override {
        // Anonymous lambda: (){ @ 42 }
        auto ast = parse("(){ @ 42 }");
        auto fd = dynamic_cast<FunctionDefNode*>(ast.get());
        ASSERT(fd && fd->isAnonymous() && fd->getParams().size() == 0);
        ASSERT(fd->getHasExplicitReturn());

        // Lambda with param: (x){ @ x * 2 }
        ast = parse("(x){ @ x * 2 }");
        fd = dynamic_cast<FunctionDefNode*>(ast.get());
        ASSERT(fd && fd->getParams().size() == 1 && fd->getParams()[0] == "x");

        // Multiple params: (a, b){ @ a + b }
        ast = parse("(a, b){ @ a + b }");
        fd = dynamic_cast<FunctionDefNode*>(ast.get());
        ASSERT(fd && fd->getParams().size() == 2);

        // Function call: print(42)
        ast = parse("print(42)");
        auto fc = dynamic_cast<FunctionCallNode*>(ast.get());
        ASSERT(fc && fc->getName() == "print" && fc->getArgs().size() == 1);

        // Multi-arg call: add(1, 2, 3)
        ast = parse("add(1, 2, 3)");
        fc = dynamic_cast<FunctionCallNode*>(ast.get());
        ASSERT(fc && fc->getArgs().size() == 3);
    }
};

int main() {
    TestRunner runner;
    runner.addTest("Functions: Definition", std::make_shared<FunctionDefinition>());
    runner.addTest("Functions: Call", std::make_shared<FunctionCall>());
    runner.addTest("Functions: Lambda", std::make_shared<LambdaExpression>());
    runner.addTest("Functions: Scope", std::make_shared<FunctionScope>());
    runner.addTest("Functions: Builtins", std::make_shared<BuiltinFunctions>());
    runner.addTest("Functions: Unknown Function", std::make_shared<UnknownFunction>());
    runner.addTest("Functions: Parser Verification", std::make_shared<ParserVerification>());
    return runner.runAll();
}

#endif // isTEST
