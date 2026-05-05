/**
 * @file src/lexer/operators.cpp
 * @brief Handler for operator tokens.
**/

#include "lexer.hpp"
#include <algorithm>

using namespace DemoLang::Tokens;


namespace DemoLang {

namespace LexerSpace {

OperatorHandler::OperatorHandler(Lexer& lexer) : BaseHandler(lexer) {}

std::shared_ptr<Token> OperatorHandler::handle() {
    char c = lexer.current();
    for (const auto& op : operators) {
        if (c == op[0] && lexer.pos() + op.length() <= lexer.getInput().length()) {
            std::string potentialOp = lexer.getInput().substr(lexer.pos(), op.length());
            if (potentialOp == op) {
                lexer.advance(op.length());
                return Lexer::getToken(TokenType::OPERATOR, op);
            }
        }
    }
    return nextHandler->handle();
}

} // namespace LexerSpace

} // namespace DemoLang
