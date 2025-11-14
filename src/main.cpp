/**
 * @file src/main.cpp
 * @brief Program entry point.
 * @author The-XiaoBai
 * @date 2025/11/14
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