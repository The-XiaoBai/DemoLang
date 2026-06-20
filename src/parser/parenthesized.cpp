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
        parser.advance();
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
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return {std::make_shared<ErrorNode>("Expected ')' after parameters"), false};
    parser.advance();

    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return {std::make_shared<ErrorNode>("Expected '{' for lambda body"), false};
    parser.advance();

    auto [body, hasReturn] = parseFunctionBody(parser);

    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return {std::make_shared<ErrorNode>("Expected '}' for lambda body"), false};
    parser.advance();

    return {body, hasReturn};
}

ParenthesizedParser::ParenthesizedParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> ParenthesizedParser::handle() {
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "(") {
        return nextHandler->handle();
    }
    parser.advance();

    // Empty-parameter lambda: (){body}
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
        auto [body, hasReturn] = parseLambda(parser);
        auto lambda = std::make_shared<FunctionDefNode>("", std::vector<std::string>{}, std::vector<std::shared_ptr<ASTNode>>{}, body, hasReturn);

        if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
            parser.advance();
            std::vector<std::shared_ptr<ASTNode>> args;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance();
            } else {
                args.push_back(parser.parseExpression());
                while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                    parser.advance();
                    args.push_back(parser.parseExpression());
                }
                if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                    return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                parser.advance();
            }
            return std::make_shared<FunctionCallNode>(lambda, args);
        }
        return lambda;
    }

    // lambda: (params){body}
    if (parser.current().type == TokenType::IDENTIFIER) {
        size_t afterParen = parser.savePosition();
        std::vector<std::string> params;
        std::vector<std::shared_ptr<ASTNode>> paramDefaults;

        auto [pName, pDef, err] = parser.parseOneParam();
        if (err) return err;
        params.push_back(pName);
        paramDefaults.push_back(pDef);

        if (parser.current().type == TokenType::OPERATOR &&
            (parser.current().value == ")" || parser.current().value == ",")) {

            while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                parser.advance();
                auto [rpName, rpDef, rerr] = parser.parseOneParam();
                if (rerr) return std::make_shared<ErrorNode>("Expected parameter name after ','");
                params.push_back(rpName);
                paramDefaults.push_back(rpDef);
            }

            auto [body, hasReturn] = parseLambda(parser);
            auto lambda = std::make_shared<FunctionDefNode>("", params, paramDefaults, body, hasReturn);

            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                parser.advance();
                std::vector<std::shared_ptr<ASTNode>> args;
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    parser.advance();
                } else {
                    args.push_back(parser.parseExpression());
                    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                        parser.advance();
                        args.push_back(parser.parseExpression());
                    }
                    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                        return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                    parser.advance();
                }
                return std::make_shared<FunctionCallNode>(lambda, args);
            }
            return lambda;
        }

        parser.restorePosition(afterParen);
    }

    // Check for end of input
    if (parser.current().type == TokenType::END)
        return std::make_shared<ErrorNode>("Unexpected end of input, expected closing parenthesis");

    // Parse as regular expression using parseExpression to preserve outer operators
    auto expr = parser.parseExpression();

    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected closing parenthesis");
    parser.advance();

    if (auto* funcDef = dynamic_cast<FunctionDefNode*>(expr.get())) {
        if (funcDef->isAnonymous() && parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
            parser.advance();
            std::vector<std::shared_ptr<ASTNode>> args;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance();
            } else {
                args.push_back(parser.parseExpression());
                while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                    parser.advance();
                    args.push_back(parser.parseExpression());
                }
                if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
                    return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                parser.advance();
            }
            return std::make_shared<FunctionCallNode>(expr, args);
        }
    }

    return expr;
}

} // namespace ParserSpace
} // namespace DemoLang
