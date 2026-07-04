/**
 * @file src/parser/functions.cpp
 * @brief Parser for function calls.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;

namespace DemoLang {
namespace ParserSpace {

FunctionCallParser::FunctionCallParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> FunctionCallParser::handle() {
    // Check if current token is an identifier (potential function name)
    if (parser.current().type != TokenType::IDENTIFIER) {
        return nextHandler->handle();
    }

    std::string funcName = parser.current().value;
    size_t savedPos = parser.savePosition();  // Save position for backtracking

    parser.advance();  // Consume identifier

    // Check if identifier is followed by '(' (function call) or '[' (index access)
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
        parser.advance();  // Consume '('
    } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "[") {
        // This is an index access, not a function call
        parser.restorePosition(savedPos);
        return nextHandler->handle();
    } else {
        // This is a standalone identifier, not a function call
        parser.restorePosition(savedPos);
        return nextHandler->handle();
    }

    // Parse arguments
    std::vector<std::shared_ptr<ASTNode>> args;

    // Check for empty argument list
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
        parser.advance();  // Consume ')'
        return std::make_shared<FunctionCallNode>(funcName, args);
    }

    // Parse first argument
    args.push_back(parser.parseExpression());

    // Propagate lexer error immediately
    if (parser.current().type == TokenType::ERROR)
        return std::make_shared<ErrorNode>(parser.current().value);

    // Parse remaining arguments separated by ','
    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
        parser.advance();  // Consume ','
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")")
            return std::make_shared<ErrorNode>("Unexpected ',' before ')'");
        args.push_back(parser.parseExpression());
        if (parser.current().type == TokenType::ERROR)
            return std::make_shared<ErrorNode>(parser.current().value);
    }

    // Propagate lexer error before checking for ')'
    if (parser.current().type == TokenType::ERROR)
        return std::make_shared<ErrorNode>(parser.current().value);

    // Expect closing parenthesis
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in function call");
    parser.advance();  // Consume ')'

    return std::make_shared<FunctionCallNode>(funcName, args);
}

} // namespace ParserSpace
} // namespace DemoLang
