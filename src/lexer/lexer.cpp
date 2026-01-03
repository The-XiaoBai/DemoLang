/**
 * @file src/lexer/lexer.cpp
 * @brief Transform input to tokens.
 * @author The-XiaoBai
 * @date 2026/01/03
**/

#include "lexer.hpp"


namespace DemoLang {

Token LexerSpace::Lexer::nextToken() {
    // Create a chain of responsibility pattern for token recognition
    Chain<Token> chain;
    // Add handlers in order of priority
    chain.addHandler(std::make_shared<EOFHandler>(*this));
    chain.addHandler(std::make_shared<WhitespaceHandler>(*this));
    chain.addHandler(std::make_shared<StringHandler>(*this));
    chain.addHandler(std::make_shared<NumberHandler>(*this));
    chain.addHandler(std::make_shared<OperatorHandler>(*this));
    chain.addHandler(std::make_shared<IdentifierHandler>(*this));
    chain.addHandler(std::make_shared<UnknownHandler>(*this));

    auto result = chain.execute();
    return *result;
}


std::vector<Token> LexerSpace::Lexer::tokenize(const std::string &input) {
    // Initialize lexer state
    this->input = input;
    this->position = 0;
    std::vector<Token> tokens;
    Token token = this->nextToken();
    
    // Process tokens until end of input
    while (token.type != TokenType::END) {
        tokens.push_back(token);
        // If there is an error, stop tokenizing
        if (token.type == TokenType::ERROR) {
            token = Token(TokenType::END, "");
            break;
        }
        token = this->nextToken();
    }
    
    // Add END token to mark completion
    tokens.push_back(token);
    return tokens;
}

} // namespace DemoLang