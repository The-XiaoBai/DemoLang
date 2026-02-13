/**
 * @file src/fileloader.cpp
 * @brief Load DemoLang code from a file and execute. Usage: <executable> <filename>
**/

#include "lexer.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

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
        // Read file content
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Cannot open file: " + filename);
        
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Split content into lines
        std::istringstream stream(content);
        std::string line;
        std::string lastResult;
        
        Lexer& lexer = Lexer::instance();
        Parser& parser = Parser::instance();
        Interpreter& interpreter = Interpreter::instance();
        
        // Process each line as separate expression
        while (std::getline(stream, line)) {
            // Skip empty lines
            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
                continue;
            }
            
            // Execute current line
            auto tokens = lexer.tokenize(line);
            auto ast = parser.parse(tokens);
            lastResult = interpreter.interpret(ast);
        }
        
        // Only print last result of the file execution
        std::cout << lastResult << std::endl;
        
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
    std::cout << "$[Argument count]: " << argc << std::endl;
    for (int i = 0; i < argc; i++) {
        std::cout << "$[Argument-" << i << "]: " << argv[i] << std::endl;
    }
    std::cout << std::endl << std::endl;

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
