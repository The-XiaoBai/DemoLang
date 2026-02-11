/**
 * @file src/main.cpp
 * @brief Program entry point.
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
    std::cout << "[DemoLang]" << std::endl << std::endl;

    // Main REPL loop
    while (true) {
        std::string input;
        std::cout << "> ";
        std::getline(std::cin, input);

        // Skip empty input
        if (input.empty()) continue;

        try {
            // Lexical analysis (tokenization) - convert input string to tokens
            Lexer& lexer = Lexer::instance();
            auto tokens = lexer.tokenize(input);

            // Syntax analysis (parsing) - convert tokens to Abstract Syntax Tree
            Parser& parser = Parser::instance();
            auto ast = parser.parse(tokens);
            
            // Semantic analysis and execution (interpretation) - evaluate AST
            Interpreter& interpreter = Interpreter::instance();
            auto result = interpreter.interpret(ast);

            // Print result
            std::cout << ">>> " << result << std::endl;
        } catch (const std::exception& e) {
            // Handle and display any processing errors
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