/**
 * @file src/lexer/lexer.cpp
 * @brief Transform input to tokens.
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#include "lexer.hpp"


namespace DemoLang {

Token LexerSpace::Lexer::nextToken() {
    Chain<Token> chain;
    chain.addHandler(std::make_shared<AHandler>(*this));

    auto result = chain.execute();
    if (!result) result = nullptr;
    return *result;
}


std::vector<Token> LexerSpace::Lexer::tokenize(const std::string &input) {
    this->input = input;
    this->position = 0;
    std::vector<Token> tokens;
    Token token = this->nextToken();
    
    while (token.type != TokenType::END) {
        tokens.push_back(token);
        token = this->nextToken();
    }
    
    tokens.push_back(token);
    return tokens;
}


std::shared_ptr<Token> LexerSpace::AHandler::handle() {
    return std::make_shared<Token>(TokenType::END, "");
}

} // namespace DemoLang