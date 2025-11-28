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


class ParserTestCase : public TestCase {
protected:
    Parser* parser;
    void setUp() override { parser = &Parser::instance(); }
    void tearDown() override { parser = nullptr; }
};


class TestUnaryOp : public ParserTestCase {
public:
    void run() override {
        std::vector<Token> tokens1 = {
            {TokenType::OPERATOR, "-"},
            {TokenType::INTEGER_LITERAL, "5"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast1 = parser->parse(tokens1);
        assert(ast1);

        auto unaryOp1 = dynamic_cast<UnaryOpNode*>(ast1.get());
        assert(unaryOp1);
        assert(unaryOp1->getOp() == "-");
        assert(dynamic_cast<IntNode*>(unaryOp1->getOperand())->getValue() == 5);

        std::vector<Token> tokens2 = {
            {TokenType::OPERATOR, "!"},
            {TokenType::INTEGER_LITERAL, "0"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast2 = parser->parse(tokens2);
        assert(ast2);

        auto unaryOp2 = dynamic_cast<UnaryOpNode*>(ast2.get());
        assert(unaryOp2);
        assert(unaryOp2->getOp() == "!");
        assert(dynamic_cast<IntNode*>(unaryOp2->getOperand())->getValue() == 0);
    }
};


class TestBinaryOp : public ParserTestCase {
public:
    void run() override {
        std::vector<Token> tokens1 = {
            {TokenType::IDENTIFIER, "a"},
            {TokenType::OPERATOR, "="},
            {TokenType::INTEGER_LITERAL, "1"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast1 = parser->parse(tokens1);
        assert(ast1);

        auto binaryOp1 = dynamic_cast<BinaryOpNode*>(ast1.get());
        assert(binaryOp1->getOp() == "=");
        assert(dynamic_cast<IdNode*>(binaryOp1->getLeft())->getName() == "a");
        assert(dynamic_cast<IntNode*>(binaryOp1->getRight())->getValue() == 1);

        std::vector<Token> tokens2 = {
            {TokenType::INTEGER_LITERAL, "3"},
            {TokenType::OPERATOR, "*"},
            {TokenType::INTEGER_LITERAL, "4"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast2 = parser->parse(tokens2);
        assert(ast2);

        auto binaryOp2 = dynamic_cast<BinaryOpNode*>(ast2.get());
        assert(binaryOp2);
        assert(binaryOp2->getOp() == "*");
        assert(dynamic_cast<IntNode*>(binaryOp2->getLeft())->getValue() == 3);
        assert(dynamic_cast<IntNode*>(binaryOp2->getRight())->getValue() == 4);

        std::vector<Token> tokens3 = {
            {TokenType::OPERATOR, "("},
            {TokenType::OPERATOR, "("},
            {TokenType::INTEGER_LITERAL, "2"},
            {TokenType::OPERATOR, "+"},
            {TokenType::INTEGER_LITERAL, "3"},
            {TokenType::OPERATOR, ")"},
            {TokenType::OPERATOR, "*"},
            {TokenType::INTEGER_LITERAL, "4"},
            {TokenType::OPERATOR, ")"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast3 = parser->parse(tokens3);
        assert(ast3);

        auto binaryOp3 = dynamic_cast<BinaryOpNode*>(ast3.get());
        assert(binaryOp3);
        assert(binaryOp3->getOp() == "*");
    }
};


class TestLiterals : public ParserTestCase {
public:
    void run() override {
        std::vector<Token> int_tokens = {
            {TokenType::INTEGER_LITERAL, "123"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> int_ast = parser->parse(int_tokens);
        assert(int_ast);

        auto intNode = dynamic_cast<IntNode*>(int_ast.get());
        assert(intNode);
        assert(intNode->getValue() == 123);
        
        std::vector<Token> float_tokens = {
            {TokenType::FLOAT_LITERAL, "1.23"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> float_ast = parser->parse(float_tokens);
        assert(float_ast);

        auto floatNode = dynamic_cast<FloatNode*>(float_ast.get());
        assert(floatNode);
        assert(floatNode->getValue() == 1.23);

        std::vector<Token> str_tokens = {
            {TokenType::STRING_LITERAL, "\"hello\""},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> str_ast = parser->parse(str_tokens);
        assert(str_ast);

        auto strNode = dynamic_cast<StringNode*>(str_ast.get());
        assert(strNode);
        assert(strNode->getValue() == "hello");
    }
};


class TestErrorHandling : public ParserTestCase {
public:
    void run() override {
        std::vector<Token> tokens = {
            {TokenType::OPERATOR, "++"},
            {TokenType::END, ""}
        };
        std::shared_ptr<ASTNode> ast = parser->parse(tokens);
        assert(ast);

        auto errorNode = dynamic_cast<ErrorNode*>(ast.get());
        assert(errorNode);
    }
};


int main() {
    TestRunner runner;
    runner.addTest("Parser: Unary Operator", std::make_shared<TestUnaryOp>());
    runner.addTest("Parser: Binary Operator", std::make_shared<TestBinaryOp>());
    runner.addTest("Parser: Literals", std::make_shared<TestLiterals>());
    runner.addTest("Parser: Error Handling", std::make_shared<TestErrorHandling>());
    runner.runAll();

    return 0;
}

#endif // isTEST