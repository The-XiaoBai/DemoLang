#ifdef isTEST

#include "test_framework.hpp"
#include "tokens.hpp"
#include "lexer.hpp"

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;


class TestLexer : public TestCase {
private:
    Lexer* lexer;

public:
    void setUp() override {
        lexer = &Lexer::instance();
    }
    
    void run() override {
        auto tokens = lexer->tokenize("");
        assert(1 == 1);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Lexer: Test Lexer", std::make_shared<TestLexer>());
    runner.runAll();

    return 0;
}

#endif // isTEST