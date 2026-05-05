/**
 * @file src/lexer/lexer.cpp
 * @brief Transform input to tokens.
**/

#include "lexer.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;


namespace DemoLang {

namespace LexerSpace {

BaseHandler::BaseHandler(Lexer& lexer) : lexer(lexer) {}

Lexer::Lexer() : position(0) {
    handlerChain.addHandler(std::make_shared<EOFHandler>(*this));
    handlerChain.addHandler(std::make_shared<WhitespaceHandler>(*this));
    handlerChain.addHandler(std::make_shared<StringHandler>(*this));
    handlerChain.addHandler(std::make_shared<NumberHandler>(*this));
    handlerChain.addHandler(std::make_shared<OperatorHandler>(*this));
    handlerChain.addHandler(std::make_shared<IdentifierHandler>(*this));
    handlerChain.addHandler(std::make_shared<UnknownHandler>(*this));
}

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
    auto result = handlerChain.execute();
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
