/**
 * @file src/main.cpp
 * @brief Program entry point.
 * @author The-XiaoBai
 * @date 2025/11/16
**/


#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <iostream>

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;
using namespace DemoLang::AST;
using namespace DemoLang::ParserSpace;
using namespace DemoLang::ValueTypes;
using namespace DemoLang::InterpreterSpace;


/**
 * @brief REPL for DemoLang.
**/
static void repl() {
    std::cout << "[DemoLang]" << std::endl;
    std::cout << std::endl;

    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input.empty()) continue;

        try {
            // Lexical analysis (tokenization)
            Lexer& lexer = Lexer::instance();
            auto tokens = lexer.tokenize(input);

            // Syntax analysis (parsing)
            Parser& parser = Parser::instance();
            auto ast = parser.parse(tokens);

            // ? DEBUG
            std::cout << "AST node type: " << typeid(*ast).name() << std::endl;
            if (auto err = dynamic_cast<ErrorNode*>(ast.get())) {
                std::cout << "Error: " << err->getMsg() << std::endl;
            } else if (auto unOp = dynamic_cast<UnaryOpNode*>(ast.get())) {
                std::cout << "Unary Operation: " << unOp->getOp() << std::endl;
                std::cout << "Operand: " << typeid(*unOp->getOperand()).name() << std::endl;
            } else if (auto binOp = dynamic_cast<BinaryOpNode*>(ast.get())) {
                std::cout << "Binary Operation: " << binOp->getOp() << std::endl;
                std::cout << "Left operand: " << typeid(*binOp->getLeft()).name() << std::endl;
                std::cout << "Right operand: " << typeid(*binOp->getRight()).name() << std::endl;
            } else if (auto id = dynamic_cast<IdNode*>(ast.get())) {
                std::cout << "Identifier: " << id->getName() << std::endl;
            } else if (auto intNode = dynamic_cast<IntNode*>(ast.get())) {
                std::cout << "Integer Literal: " << intNode->getValue() << std::endl;
            } else if (auto floatNode = dynamic_cast<FloatNode*>(ast.get())) {
                std::cout << "Float Literal: " << floatNode->getValue() << std::endl;
            } else if (auto strNode = dynamic_cast<StringNode*>(ast.get())) {
                std::cout << "String Literal: " << strNode->getValue() << std::endl;
            } else {
                std::cout << "Unknown AST node type" << std::endl;
            }
            std::cout << std::endl;
            
            // Semantic analysis and execution (interpretation)
            Interpreter& interpreter = Interpreter::instance();
            auto result = interpreter.interpret(ast);

            // Print result
            std::cout << ">>> " << result << std::endl;
        } catch (const std::exception& e) {
            // Throw error
            std::cerr << "Processing Error: " << e.what() << std::endl;
        }
    }
}


/**
 * @brief Main function.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code.
**/
int main(int argc, char* argv[]) {
    repl();

    return 0;
}