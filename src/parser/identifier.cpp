/**
 * @file src/parser/identifier.cpp
 * @brief Parser for identifiers and index access.
**/

#include "parser.hpp"
#include "utils.hpp"

namespace DemoLang {

ParserSpace::IdentifierParser::IdentifierParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::IdentifierParser::handle() {
    if (parser.current().type != TokenType::IDENTIFIER) {
        return nextHandler->handle();
    }

    std::string idName = parser.current().value;
    parser.advance(); // Consume identifier

    // Check for index access: identifier[index]
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "[") {
        parser.advance(); // Consume '['
        auto indexExpr = parser.parseExpression();
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "]")
            return std::make_shared<ErrorNode>("Expected ']' in index access");
        parser.advance(); // Consume ']'

        // Check for chained index: a[0][1][2]
        auto object = std::make_shared<IndexNode>(std::make_shared<IdNode>(idName), indexExpr);
        while (parser.current().type == TokenType::OPERATOR && parser.current().value == "[") {
            parser.advance(); // Consume '['
            auto nextIndex = parser.parseExpression();
            if (parser.current().type != TokenType::OPERATOR || parser.current().value != "]")
                return std::make_shared<ErrorNode>("Expected ']' in index access");
            parser.advance(); // Consume ']'
            object = std::make_shared<IndexNode>(object, nextIndex);
        }
        return object;
    }

    return std::make_shared<IdNode>(idName);
}

} // namespace DemoLang
