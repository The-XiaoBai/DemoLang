/**
 * @file src/lexer/eof.cpp
 * @brief Handler for end-of-file tokens.
**/

#include "lexer.hpp"

using namespace DemoLang::Tokens;

namespace DemoLang {

namespace LexerSpace {

EOFHandler::EOFHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> EOFHandler::handle() {
    if (lexer.current() == '\0')
        return Lexer::getToken(TokenType::END);
    return nextHandler->handle();
}

} // namespace LexerSpace

} // namespace DemoLang
