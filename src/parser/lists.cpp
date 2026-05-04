/**
 * @file src/parser/lists.cpp
 * @brief Parser for list literals.
**/

#include "parser.hpp"
#include "utils.hpp"

namespace DemoLang {

ParserSpace::ListParser::ListParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::ListParser::handle() {
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "[") {
        return nextHandler->handle();
    }
    parser.advance(); // Consume '['

    std::vector<std::shared_ptr<ASTNode>> elements;

    // Empty list
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "]") {
        parser.advance(); // Consume ']'
        return std::make_shared<ListNode>(elements);
    }

    // Parse elements until ']'
    while (!(parser.current().type == TokenType::OPERATOR && parser.current().value == "]")) {
        elements.push_back(parser.parseExpression());
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
            parser.advance(); // Consume ','
        } else {
            break;
        }
    }

    // Expect closing ']'
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "]")
        return std::make_shared<ErrorNode>("Expected ']' in list literal");
    parser.advance(); // Consume ']'

    return std::make_shared<ListNode>(elements);
}

} // namespace DemoLang
