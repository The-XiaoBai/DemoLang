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

static std::shared_ptr<ASTNode> parseLambda(Parser& parser) {
    // Expect ')' after parameters
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")")
        return std::make_shared<ErrorNode>("Expected ')' after parameters");
    parser.advance(); // Consume ')'

    // Expect '{' for lambda body
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{")
        return std::make_shared<ErrorNode>("Expected '{' for lambda body");
    parser.advance(); // Consume '{'

    // Parse lambda body
    std::shared_ptr<ASTNode> body;
    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "@") {
        parser.advance(); // Consume '@'
        body = parser.parseExpression();
    } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
        body = std::make_shared<StringNode>("");
    } else {
        body = parser.parseExpression();
    }

    // Expect closing '}'
    if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}")
        return std::make_shared<ErrorNode>("Expected '}' for lambda body");
    parser.advance(); // Consume '}'

    return body;
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
        auto body = parseLambda(parser);
        return std::make_shared<FunctionDefNode>("", std::vector<std::string>{}, std::vector<std::shared_ptr<ASTNode>>{}, body);
    }

    // Check for lambda: (params){body}
    if (parser.current().type == TokenType::IDENTIFIER) {
        std::vector<std::string> params;
        std::vector<std::shared_ptr<ASTNode>> paramDefaults;

        auto [pName, pDef, err] = parser.parseOneParam();
        if (err) return err;
        params.push_back(pName);
        paramDefaults.push_back(pDef);

        while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
            parser.advance(); // Consume ','
            auto [rpName, rpDef, rerr] = parser.parseOneParam();
            if (rerr) return std::make_shared<ErrorNode>("Expected parameter name after ','");
            params.push_back(rpName);
            paramDefaults.push_back(rpDef);
        }

        auto body = parseLambda(parser);
        return std::make_shared<FunctionDefNode>("", params, paramDefaults, body);
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
