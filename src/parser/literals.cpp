/**
 * @file src/parser/literals.cpp
 * @brief Fallback parser for literal values.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;


namespace DemoLang {

ParserSpace::LiteralFallbackParser::LiteralFallbackParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::LiteralFallbackParser::handle() {
    Token token = parser.current();
    
    // Only handle literal types, otherwise return nullptr to pass to next handler
    switch (token.type) {
        case TokenType::STRING_LITERAL: {
            parser.advance();
            if (token.value.size() >= 2) {
                char first = token.value.front(), last = token.value.back();
                if ((first == '"' && last == '"') || (first == '\'' && last == '\''))
                    return std::make_shared<StringNode>(token.value.substr(1, token.value.size() - 2));
            }
            return std::make_shared<ErrorNode>("Invalid string: " + token.value);
        }
        case TokenType::INTEGER_LITERAL: {
            parser.advance();
            try {
                long long val = std::stoll(token.value);
                return std::make_shared<IntNode>(val);
            } catch (...) {
                return std::make_shared<ErrorNode>("Invalid integer: " + token.value);
            }
        }
        case TokenType::FLOAT_LITERAL: {
            parser.advance();
            try {
                char* end;
                double val = std::strtold(token.value.c_str(), &end);
                if (end != token.value.c_str() + token.value.length() || errno == ERANGE)
                    return std::make_shared<ErrorNode>("Invalid float");
                return std::make_shared<FloatNode>(val);
            } catch (...) {
                return std::make_shared<ErrorNode>("Invalid float: " + token.value);
            }
        }
        case TokenType::END:
            return std::make_shared<ErrorNode>("Unexpected end of input");
        case TokenType::ERROR:
            return std::make_shared<ErrorNode>(token.value);
        default:
            return nullptr;
    }
}

} // namespace DemoLang
