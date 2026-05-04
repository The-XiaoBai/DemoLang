/**
 * @file src/lexer/string.cpp
 * @brief Handler for string literal tokens.
**/

#include "lexer.hpp"


namespace DemoLang {

namespace LexerSpace {

StringHandler::StringHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> StringHandler::handle() {
    char c = lexer.current();
    if (c != '\"' && c != '\'')
        return nextHandler->handle();

    char quote = c;
    std::string value;
    value += quote;
    lexer.advance();

    while (lexer.current() != quote && lexer.current() != '\0') {
        if (lexer.current() == '\\') {
            value += lexer.current();
            lexer.advance();
            if (lexer.current() != '\0')
                value += lexer.current();
        } else {
            value += lexer.current();
        }
        lexer.advance();
    }

    if (lexer.current() == quote) {
        value += quote;
        lexer.advance();
        return Lexer::getToken(TokenType::STRING_LITERAL, value);
    }
    return Lexer::getToken(TokenType::ERROR, "Unterminated string: " + value);
}

} // namespace LexerSpace

} // namespace DemoLang
