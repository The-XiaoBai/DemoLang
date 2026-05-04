/**
 * @file src/parser/statements.cpp
 * @brief Parser for if, while, break, continue statements.
**/

#include "parser.hpp"
#include "utils.hpp"

namespace DemoLang {

ParserSpace::IfParser::IfParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::IfParser::handle() {
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "?") {
        return nextHandler->handle();
    }
    parser.advance(); // Consume '?'

    std::vector<std::shared_ptr<ASTNode>> conditions;
    std::vector<std::shared_ptr<ASTNode>> bodies;

    // Parse first if branch: (condition){body}
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
        return std::make_shared<ErrorNode>("Expected '(' after '?'");
    parser.advance(); // Consume '('
    conditions.push_back(parser.parseExpression());
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in if condition");
    parser.advance(); // Consume ')'

    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return std::make_shared<ErrorNode>("Expected '{' for if body");
    parser.advance(); // Consume '{'
    bodies.push_back(parser.parseExpression());
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return std::make_shared<ErrorNode>("Expected '}' for if body");
    parser.advance(); // Consume '}'

    // Parse else-if branches: ??(condition){body}
    while (parser.current().type == TokenType::OPERATOR && parser.current().value == "??") {
        parser.advance(); // Consume '??'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
            return std::make_shared<ErrorNode>("Expected '(' after '??'");
        parser.advance(); // Consume '('
        conditions.push_back(parser.parseExpression());
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
            return std::make_shared<ErrorNode>("Expected ')' in else-if condition");
        parser.advance(); // Consume ')'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
            return std::make_shared<ErrorNode>("Expected '{' for else-if body");
        parser.advance(); // Consume '{'
        bodies.push_back(parser.parseExpression());
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
            return std::make_shared<ErrorNode>("Expected '}' for else-if body");
        parser.advance(); // Consume '}'
    }

    // Parse optional else branch: :{body}
    std::shared_ptr<ASTNode> elseBody = nullptr;
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ":") {
        parser.advance(); // Consume ':'

        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
            return std::make_shared<ErrorNode>("Expected '{' for else body");
        parser.advance(); // Consume '{'
        elseBody = parser.parseExpression();
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
            return std::make_shared<ErrorNode>("Expected '}' for else body");
        parser.advance(); // Consume '}'
    }

    return std::make_shared<IfNode>(conditions, bodies, elseBody);
}

ParserSpace::WhileParser::WhileParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::WhileParser::handle() {
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "$") {
        return nextHandler->handle();
    }
    parser.advance(); // Consume '$'

    // Parse condition
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(")
        return std::make_shared<ErrorNode>("Expected '(' after '$'");
    parser.advance(); // Consume '('
    auto condition = parser.parseExpression();
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' in while condition");
    parser.advance(); // Consume ')'

    // Parse body
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return std::make_shared<ErrorNode>("Expected '{' for while body");
    parser.advance(); // Consume '{'
    auto body = parser.parseExpression();
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return std::make_shared<ErrorNode>("Expected '}' for while body");
    parser.advance(); // Consume '}'

    return std::make_shared<WhileNode>(condition, body);
}

ParserSpace::LoopControlParser::LoopControlParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::LoopControlParser::handle() {
    if (parser.current().type != TokenType::OPERATOR) {
        return nextHandler->handle();
    }

    if (parser.current().value == "##") {
        parser.advance(); // Consume '##'
        return std::make_shared<BreakNode>();
    }
    if (parser.current().value == "#") {
        parser.advance(); // Consume '#'
        return std::make_shared<ContinueNode>();
    }

    return nextHandler->handle();
}

} // namespace DemoLang
