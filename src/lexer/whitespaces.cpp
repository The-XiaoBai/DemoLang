/**
 * @file src/lexer/whitespaces.cpp
 * @brief Handler for whitespace skipping.
**/

#include "lexer.hpp"
#include <algorithm>

using namespace DemoLang::Tokens;


namespace DemoLang {

namespace LexerSpace {

WhitespaceHandler::WhitespaceHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> WhitespaceHandler::handle() {
    while (std::find(whitespaces.begin(), whitespaces.end(), std::string(1, lexer.current())) != whitespaces.end())
        lexer.advance();
    return nextHandler->handle();
}

} // namespace LexerSpace

} // namespace DemoLang
