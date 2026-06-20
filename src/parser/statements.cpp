/**
 * @file src/parser/statements.cpp
 * @brief Parser for if, while, break, continue statements.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;

namespace DemoLang {
namespace ParserSpace {

IfParser::IfParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> IfParser::handle() {
    // Syntax: ?(condition){body} ?? (condition){body} : {elseBody}
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "?") {
        return nextHandler->handle();
    }
    parser.advance();  // Consume '?'

    std::vector<std::shared_ptr<ASTNode>> conditions;
    std::vector<std::shared_ptr<ASTNode>> bodies;

    // Parse condition in parentheses
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
        return std::make_shared<ErrorNode>("Expected '(' after '?'");
    parser.advance();  // Consume '('
    conditions.push_back(parser.parseExpression());
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in if condition");
    parser.advance();  // Consume ')'

    // Parse body in braces
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return std::make_shared<ErrorNode>("Expected '{' for if body");
    parser.advance();  // Consume '{'
    bodies.push_back(parser.parseExpression());
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return std::make_shared<ErrorNode>("Expected '}' for if body");
    parser.advance();  // Consume '}'

    // Parse optional else-if clauses: ??(condition){body}
    while (parser.current().type == TokenType::OPERATOR && parser.current().value == "??") {
        parser.advance();  // Consume '??'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
            return std::make_shared<ErrorNode>("Expected '(' after '??'");
        parser.advance();  // Consume '('
        conditions.push_back(parser.parseExpression());
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
            return std::make_shared<ErrorNode>("Expected ')' in else-if condition");
        parser.advance();  // Consume ')'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
            return std::make_shared<ErrorNode>("Expected '{' for else-if body");
        parser.advance();  // Consume '{'
        bodies.push_back(parser.parseExpression());
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
            return std::make_shared<ErrorNode>("Expected '}' for else-if body");
        parser.advance();  // Consume '}'
    }

    // Parse optional else clause: :{elseBody}
    std::shared_ptr<ASTNode> elseBody = nullptr;
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ":") {
        parser.advance();  // Consume ':'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
            return std::make_shared<ErrorNode>("Expected '{' for else body");
        parser.advance();  // Consume '{'
        elseBody = parser.parseExpression();
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
            return std::make_shared<ErrorNode>("Expected '}' for else body");
        parser.advance();  // Consume '}'
    }

    return std::make_shared<IfNode>(conditions, bodies, elseBody);
}

WhileParser::WhileParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> WhileParser::handle() {
    // Syntax: ^(condition){body}
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "^") {
        return nextHandler->handle();
    }
    parser.advance();  // Consume '^'

    // Parse condition in parentheses
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
        return std::make_shared<ErrorNode>("Expected '(' after '^'");
    parser.advance();  // Consume '('
    auto condition = parser.parseExpression();
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in while condition");
    parser.advance();  // Consume ')'

    // Parse body in braces
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return std::make_shared<ErrorNode>("Expected '{' for while body");
    parser.advance();  // Consume '{'
    auto body = parser.parseExpression();
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return std::make_shared<ErrorNode>("Expected '}' for while body");
    parser.advance();  // Consume '}'

    return std::make_shared<WhileNode>(condition, body);
}

LoopControlParser::LoopControlParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> LoopControlParser::handle() {
    // Syntax: # for continue, ## for break
    if (parser.current().type != TokenType::OPERATOR) {
        return nextHandler->handle();
    }

    if (parser.current().value == "##") {
        parser.advance();  // Consume '##'
        return std::make_shared<LoopControlNode>(LoopControlType::Break);
    }
    if (parser.current().value == "#") {
        parser.advance();  // Consume '#'
        return std::make_shared<LoopControlNode>(LoopControlType::Continue);
    }

    return nextHandler->handle();
}

} // namespace ParserSpace
} // namespace DemoLang
