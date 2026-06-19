/**
 * @file src/fileloader.cpp
 * @brief Load DemoLang code from a file and execute. Usage: <executable> <filename>
**/

#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <fstream>

using namespace DemoLang;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;
using namespace DemoLang::AST;
using namespace DemoLang::ParserSpace;
using namespace DemoLang::ValueTypes;
using namespace DemoLang::InterpreterSpace;

/**
 * @brief Execute a file.
 * @param filename Path to the file to execute.
**/
static void executeFile(const std::string& filename) {
    try {
        // Read entire file content into a single string
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
        
        std::string content((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        file.close();
        
        // Handle empty file gracefully
        if (content.empty()) {
            return;
        }
        
        Lexer& lexer = Lexer::instance();
        Parser& parser = Parser::instance();
        Interpreter& interpreter = Interpreter::instance();
        
        // Tokenize, parse, and interpret the entire file as one program
        // This single-pass approach correctly handles multi-line constructs
        // (while loops, function definitions, etc.)
        auto tokens = lexer.tokenize(content);
        auto ast = parser.parse(tokens);
        if (!ast) {
            std::cerr << "Error: Failed to parse file" << std::endl;
            return;
        }
        auto lastResult = interpreter.interpret(ast);
        
        if (!lastResult.empty()) {
            std::cout << lastResult << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

/**
 * @brief Load a file and execute it.
 * @param argc Argument count.
 * @param argv Argument vector.
 */
void load(int argc, char* argv[]) {
    if (argc == 0) {
        std::cerr << "Environment does not support!" << std::endl;
    } else if (argc == 1) {
        std::cerr << "No file specified!" << std::endl;
    } else if (argc == 2) {
        executeFile(argv[1]);
    } else {
        std::cerr << "Too many arguments!" << std::endl;
    }
}

/**
 * @brief Main function.
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Exit status code.
**/
int main(int argc, char* argv[]) {
    load(argc, argv);

    return 0;
}
