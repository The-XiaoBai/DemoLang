/**
 * @file src/lexer/lexer.cpp
 * @brief Transform input to tokens.
 * @author The-XiaoBai
 * @date 2025/11/15
**/

#include "lexer.hpp"


namespace DemoLang {

Token LexerSpace::Lexer::nextToken() {
    Chain<Token> chain;
    chain.addHandler(std::make_shared<EOFHandler>(*this));
    chain.addHandler(std::make_shared<WhitespaceHandler>(*this));
    chain.addHandler(std::make_shared<StringHandler>(*this));
    chain.addHandler(std::make_shared<NumberHandler>(*this));
    chain.addHandler(std::make_shared<OperatorHandler>(*this));
    chain.addHandler(std::make_shared<KeywordHandler>(*this));
    chain.addHandler(std::make_shared<IdentifierHandler>(*this));
    chain.addHandler(std::make_shared<UnknownHandler>(*this));

    auto result = chain.execute();
    return *result;
}


std::vector<Token> LexerSpace::Lexer::tokenize(const std::string &input) {
    this->input = input;
    this->position = 0;
    std::vector<Token> tokens;
    Token token = this->nextToken();
    
    while (token.type != TokenType::END) {
        tokens.push_back(token);
        // If there is an error, stop tokenizing
        if (token.type == TokenType::ERROR) {
            token = Token(TokenType::END, "");
            break;
        }
        token = this->nextToken();
    }
    
    tokens.push_back(token);
    return tokens;
}

} // namespace DemoLang