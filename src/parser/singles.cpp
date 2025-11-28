/**
 * @file src/parser/singles.cpp
 * @brief Parser implementation for single tokens.
 * @author The-XiaoBai
 * @date 2025/11/16
**/

#include "parser.hpp"

namespace DemoLang {

std::shared_ptr<ASTNode> ParserSpace::PrimaryParser::handle() {
    Token token = parser.current();

    switch (token.type) {
        // Handle string literals
        case TokenType::STRING_LITERAL:
            parser.advance();
            if (token.value.size() >= 2) {
                char first = token.value.front(), last = token.value.back();
                if ((first == '\"' && last == '\"') || (first == '\'' && last == '\''))
                    return std::make_shared<StringNode>(token.value.substr(1, token.value.size() - 2));
            }
            return std::make_shared<ErrorNode>("Invalid string: " + token.value);
        // Handle integer literals
        case TokenType::INTEGER_LITERAL:
            parser.advance();
            try {
                long long val = std::stoll(token.value);
                return std::make_shared<IntNode>(static_cast<long long>(val));
            } catch (...) { return std::make_shared<ErrorNode>("Invalid integer: " + token.value); }
        // Handle float literals
        case TokenType::FLOAT_LITERAL:
            parser.advance();
            try {
                char* end;
                double val = std::strtold(token.value.c_str(), &end);
                if (end != token.value.c_str() + token.value.length() || errno == ERANGE)
                    return std::make_shared<ErrorNode>("Invalid float");
                return std::make_shared<FloatNode>(val);
            } catch (...) { return std::make_shared<ErrorNode>("Invalid float: " + token.value); }
        // Handle identifiers
        case TokenType::IDENTIFIER:
            parser.advance();
            return std::make_shared<IdNode>(token.value);
        // Handle parenthesized expressions
        case TokenType::OPERATOR:
            if (token.value == "(") {
                parser.advance();

                auto expr = parser.parseExpression();
                if (!expr) return std::make_shared<ErrorNode>("Empty expression in parentheses");        
                if (!parser.match(TokenType::OPERATOR, ")"))
                    return std::make_shared<ErrorNode>("Expected closing parenthesis");
                
                return expr;
            }
            return std::make_shared<ErrorNode>("Unexpected operator: " + token.value);
        // No valid primary expression found
        default:
            return std::make_shared<ErrorNode>("Unexpected token: " + token.value);
    }
}

} // namespace DemoLang