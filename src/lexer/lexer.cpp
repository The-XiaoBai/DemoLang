/**
 * @file src/lexer/lexer.cpp
 * @brief Transform input to tokens.
**/

#include "lexer.hpp"


namespace DemoLang {

namespace LexerSpace {

BaseHandler::BaseHandler(Lexer& lexer) : lexer(lexer) {}

Lexer::Lexer() : position(0) {}

std::string Lexer::getInput() const { return input; }
size_t Lexer::pos() const { return position; }
char Lexer::current() const { return position >= input.length() ? '\0' : input[position]; }
void Lexer::advance(size_t step) { position += step; }

std::shared_ptr<Token> Lexer::getToken(TokenType type, const std::string& value) {
    auto key = std::to_string(static_cast<int>(type)) + ":" + value;
    return FlyweightFactory<std::string, Token>::instance().getFlyweight(
        key, [type, &value] { return std::make_shared<Token>(type, value); });
}

Token Lexer::nextToken() {
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


std::vector<Token> Lexer::tokenize(const std::string &input) {
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

} // namespace LexerSpace

} // namespace DemoLang
