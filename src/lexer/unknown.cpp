/**
 * @file src/lexer/unknown.cpp
 * @brief Handler for unrecognized characters (error).
**/

#include "lexer.hpp"


namespace DemoLang {

namespace LexerSpace {

UnknownHandler::UnknownHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> UnknownHandler::handle() {
    std::string value;
    value += lexer.current();
    lexer.advance();
    return Lexer::getToken(TokenType::ERROR, "Unknown character: " + value);
}

} // namespace LexerSpace

} // namespace DemoLang
