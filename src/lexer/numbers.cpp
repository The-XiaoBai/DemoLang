/**
 * @file src/lexer/numbers.cpp
 * @brief Handler for numeric literals (int / float).
**/

#include "lexer.hpp"
#include <cctype>


namespace DemoLang {

namespace LexerSpace {

NumberHandler::NumberHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> NumberHandler::handle() {
    char c = lexer.current();
    if (!isdigit(static_cast<unsigned char>(c)))
        return nextHandler->handle();

    std::string value;
    bool hasDecimal = false;

    while (isdigit(static_cast<unsigned char>(lexer.current()))) {
        value += lexer.current();
        lexer.advance();
    }

    if (lexer.current() == '.') {
        hasDecimal = true;
        value += lexer.current();
        lexer.advance();

        if (!isdigit(static_cast<unsigned char>(lexer.current())))
            return Lexer::getToken(TokenType::ERROR, "Invalid float: " + value);

        while (isdigit(static_cast<unsigned char>(lexer.current()))) {
            value += lexer.current();
            lexer.advance();
        }

        if (lexer.current() == '.')
            return Lexer::getToken(TokenType::ERROR, "Multiple decimal points");
    }

    return Lexer::getToken(hasDecimal ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL, value);
}

} // namespace LexerSpace

} // namespace DemoLang
