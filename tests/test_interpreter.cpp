#ifdef isTEST

#include "test_framework.hpp"
#include "ast.hpp"
#include "builtins.hpp"
#include "interpreter.hpp"

using namespace DemoLang;
using namespace DemoLang::AST;
using namespace DemoLang::ValueTypes;
using namespace DemoLang::InterpreterSpace;


class TestInterpreter : public TestCase {
private:
    Interpreter* interpreter;

public:
    void setUp() override {
        interpreter = &Interpreter::instance();
    }
    
    void run() override {
        std::string resultNot = interpreter->interpret(nullptr);
        assert(1 == 1);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Interpreter: Test Interpreter", std::make_shared<TestInterpreter>());
    runner.runAll();

    return 0;
}

#endif // isTEST