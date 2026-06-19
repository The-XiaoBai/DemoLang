/**
 * @file src/parser/parenthesized.cpp
 * @brief Parser for parenthesized expressions and lambdas.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;


namespace DemoLang {
namespace ParserSpace {

std::pair<std::shared_ptr<ASTNode>, bool> parseFunctionBody(Parser& parser) {
    std::shared_ptr<ASTNode> body;
    bool hasReturn = false;
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "@") {
        parser.advance(); // Consume '@'
        hasReturn = true;
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
            body = std::make_shared<ErrorNode>("Expected expression after '@' return marker");
        } else {
            body = parser.parseExpression();
        }
    } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
        body = std::make_shared<StringNode>("");
    } else {
        body = parser.parseExpression();
    }
    return {body, hasReturn};
}

static std::pair<std::shared_ptr<ASTNode>, bool> parseLambda(Parser& parser) {
    // Expect ')' after parameters
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return {std::make_shared<ErrorNode>("Expected ')' after parameters"), false};
    parser.advance(); // Consume ')'

    // Expect '{' for lambda body
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return {std::make_shared<ErrorNode>("Expected '{' for lambda body"), false};
    parser.advance(); // Consume '{'

    auto [body, hasReturn] = parseFunctionBody(parser);

    // Expect closing '}'
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return {std::make_shared<ErrorNode>("Expected '}' for lambda body"), false};
    parser.advance(); // Consume '}'

    return {body, hasReturn};
}

} // namespace ParserSpace

ParserSpace::ParenthesizedParser::ParenthesizedParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParserSpace::ParenthesizedParser::handle() {
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(") {
        return nextHandler->handle();
    }
    parser.advance(); // Consume '('

    // Check for empty-parameter lambda: (){body}
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
        auto [body, hasReturn] = parseLambda(parser);
        auto lambda = std::make_shared<FunctionDefNode>("", std::vector<std::string>{}, std::vector<std::shared_ptr<ASTNode>>{}, body, hasReturn);

        // Check for immediate lambda call: (lambda)(args)
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
            parser.advance(); // Consume '('
            std::vector<std::shared_ptr<ASTNode>> args;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance(); // Consume ')'
            } else {
                args.push_back(parser.parseExpression());
                while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                    parser.advance();
                    args.push_back(parser.parseExpression());
                }
                if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                    return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                parser.advance(); // Consume ')'
            }
            return std::make_shared<FunctionCallNode>(lambda, args);
        }
        return lambda;
    }

    // Check for lambda: (params){body}
    // Distinguish from regular expressions like (x+y) by checking that
    // after the first IDENTIFIER, the next token is ')' or ',' (lambda) vs
    // an operator like '+', '*', etc. (regular expression).
    if (parser.current().type == TokenType::IDENTIFIER) {
        size_t afterParen = parser.savePosition();  // position just after '('
        std::vector<std::string> params;
        std::vector<std::shared_ptr<ASTNode>> paramDefaults;

        auto [pName, pDef, err] = parser.parseOneParam();
        if (err) return err;
        params.push_back(pName);
        paramDefaults.push_back(pDef);

        // After first param, check if next token is ')' or ',' — only then it's a lambda
        if (parser.current().type == TokenType::OPERATOR &&
            (parser.current().value == ")" || parser.current().value == ",")) {

            while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                parser.advance(); // Consume ','
                auto [rpName, rpDef, rerr] = parser.parseOneParam();
                if (rerr) return std::make_shared<ErrorNode>("Expected parameter name after ','");
                params.push_back(rpName);
                paramDefaults.push_back(rpDef);
            }

            auto [body, hasReturn] = parseLambda(parser);
            auto lambda = std::make_shared<FunctionDefNode>("", params, paramDefaults, body, hasReturn);

            // Check for immediate lambda call: (lambda)(args)
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                parser.advance(); // Consume '('
                std::vector<std::shared_ptr<ASTNode>> args;
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    parser.advance(); // Consume ')'
                } else {
                    args.push_back(parser.parseExpression());
                    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                        parser.advance();
                        args.push_back(parser.parseExpression());
                    }
                    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                        return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                    parser.advance(); // Consume ')'
                }
                return std::make_shared<FunctionCallNode>(lambda, args);
            }
            return lambda;
        }

        // Not a lambda — restore to right after '(' and fall through to expression parsing
        parser.restorePosition(afterParen);
    }

    // Check for end of input
    if (parser.current().type == TokenType::END)
        return std::make_shared<ErrorNode>("Unexpected end of input, expected closing parenthesis");

    // Parse as regular expression using parseExpressionInternal to preserve outer operators
    auto expr = parser.parseExpressionInternal();

    // Expect closing parenthesis
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected closing parenthesis");
    parser.advance(); // Consume ')'

    // Check for lambda call: (lambda)(args)
    if (auto* funcDef = dynamic_cast<FunctionDefNode*>(expr.get())) {
        if (funcDef->isAnonymous() && parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
            parser.advance(); // Consume '('

            std::vector<std::shared_ptr<ASTNode>> args;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance(); // Consume ')'
            } else {
                args.push_back(parser.parseExpression());
                while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                    parser.advance();
                    args.push_back(parser.parseExpression());
                }
                if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                    return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                parser.advance(); // Consume ')'
            }
            return std::make_shared<FunctionCallNode>(expr, args);
        }
    }

    return expr;
}

} // namespace DemoLang
