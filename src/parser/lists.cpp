/**
 * @file src/parser/lists.cpp
 * @brief Parser for list literals.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;

namespace DemoLang {
namespace ParserSpace {

ListParser::ListParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ListParser::handle() {
    // Check for list literal syntax: [elem1, elem2, ...]
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "[") {
        return nextHandler->handle();
    }
    parser.advance();  // Consume '['

    std::vector<std::shared_ptr<ASTNode>> elements;

    // Check for empty list
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "]") {
        parser.advance();  // Consume ']'
        return std::make_shared<ListNode>(elements);
    }

    // Parse list elements separated by ','
    while (!(parser.current().type == TokenType::OPERATOR && parser.current().value == "]")) {
        elements.push_back(parser.parseExpression());
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
            parser.advance();  // Consume ','
        } else {
            break;  // No more elements
        }
    }

    // Expect closing bracket
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "]")
        return std::make_shared<ErrorNode>("Expected ']' in list literal");
    parser.advance();  // Consume ']'

    return std::make_shared<ListNode>(elements);
}

} // namespace ParserSpace
} // namespace DemoLang
