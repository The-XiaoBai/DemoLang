/**
 * @file src/parser/functions.cpp
 * @brief Parser for function calls.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;


namespace DemoLang {

ParserSpace::FunctionCallParser::FunctionCallParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::FunctionCallParser::handle() {
    if (parser.current().type != TokenType::IDENTIFIER) {
        return nextHandler->handle();
    }

    // Check if next token is '(' to confirm this is a function call
    // IMPORTANT: Only consume the identifier if we confirm it's a function call
    // This allows IdentifierParser to handle plain identifiers
    std::string funcName = parser.current().value;
    size_t savedPos = parser.savePosition();  // Save position in case it's not a function call
    
    parser.advance(); // Consume identifier

    // Check for function call: identifier(args) or identifier[...]()
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
        parser.advance(); // Consume '('
    } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "[") {
        // Index access first, then will be followed by () for function call
        // Restore position and let IdentifierParser handle this
        parser.restorePosition(savedPos);
        return nextHandler->handle();
    } else {
        // Not a function call, restore position and let next handler process
        parser.restorePosition(savedPos);
        return nextHandler->handle();
    }

    // Parse arguments
    std::vector<std::shared_ptr<ASTNode>> args;

    // Empty argument list
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
        parser.advance(); // Consume ')'
        return std::make_shared<FunctionCallNode>(funcName, args);
    }

    // Parse first argument
    auto [firstArg, firstErr] = parser.parseOneArg();
    if (firstErr) return firstErr;
    args.push_back(firstArg);

    // Parse remaining arguments
    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
        parser.advance(); // Consume ','
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")")
            return std::make_shared<ErrorNode>("Unexpected ',' before ')'");
        auto [arg, err] = parser.parseOneArg();
        if (err) return err;
        args.push_back(arg);
    }

    // Expect closing ')'
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in function call");
    parser.advance(); // Consume ')'

    return std::make_shared<FunctionCallNode>(funcName, args);
}

} // namespace DemoLang
