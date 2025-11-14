#ifdef isTEST

#include "test_framework.hpp"
#include "tokens.hpp"
#include "ast.hpp"
#include "parser.hpp"
#include <memory>
#include <vector>

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::AST;
using namespace DemoLang::ParserSpace;


class TestParser : public TestCase {
private:
    Parser* parser;

public:
    void setUp() override {
        parser = &Parser::instance();
    }

    void run() override {
        auto ast = parser->parse(std::vector<Token>());
        assert(1 == 1);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Parser: Test Parser", std::make_shared<TestParser>());
    runner.runAll();

    return 0;
}

#endif // isTEST