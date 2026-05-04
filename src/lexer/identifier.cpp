/**
 * @file src/lexer/identifier.cpp
 * @brief Handler for identifier tokens.
**/

#include "lexer.hpp"
#include <cctype>


namespace DemoLang {

namespace LexerSpace {

IdentifierHandler::IdentifierHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> IdentifierHandler::handle() {
    char c = lexer.current();
    if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
        std::string value;
        while (isalnum(static_cast<unsigned char>(lexer.current())) || lexer.current() == '_') {
            value += lexer.current();
            lexer.advance();
        }
        return Lexer::getToken(TokenType::IDENTIFIER, value);
    }
    return nextHandler->handle();
}

} // namespace LexerSpace

} // namespace DemoLang
