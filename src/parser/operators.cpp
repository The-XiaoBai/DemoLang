/**
 * @file src/parser/operators.cpp
 * @brief Parser implementation for operators.
**/

#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang::AST;
using namespace DemoLang::Tokens;

namespace DemoLang {
namespace ParserSpace {

std::shared_ptr<ASTNode> UnaryParser::handle() {
    // Check if current token is a unary operator (e.g., ! or -)
    Token token = parser.current();
    if (token.type == TokenType::OPERATOR && std::find(operators.begin(), operators.end(), token.value) != operators.end()) {
        parser.advance();  // Consume operator
        auto operand = nextHandler->handle();  // Parse operand
        if (!operand) return std::make_shared<ErrorNode>("Expected expression after: " + token.value);
        return std::make_shared<UnaryOpNode>(token.value, std::move(operand));
    }
    return nextHandler->handle();  // Not a unary operator, try next handler
}

std::shared_ptr<ASTNode> BinaryParser::handle() {
    // Parse left operand first
    auto left = nextHandler->handle();
    if (!left) return std::make_shared<ErrorNode>("Left part can not be parsed");

    // Propagate error immediately - don't try to parse operators on an error
    if (dynamic_cast<ErrorNode*>(left.get())) return left;

    // Try to parse binary operators in a loop (for operator precedence)
    while (true) {
        Token token = parser.current();
        if (token.type != TokenType::OPERATOR
            || std::find(operators.begin(), operators.end(), token.value) == operators.end())
            break;  // Not a binary operator we handle

        std::string op = token.value;
        parser.advance();  // Consume operator

        // Special case: function definition with identifier = (params) { body }
        if (op == "=" && dynamic_cast<IdNode*>(left.get()) != nullptr) {
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                std::string funcName = dynamic_cast<IdNode*>(left.get())->getName();
                parser.advance();

                std::vector<std::string> params;
                std::vector<std::shared_ptr<ASTNode>> paramDefaults;

                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    parser.advance();
                } else {
                    auto [pName, pDef, err] = parser.parseOneParam();
                    if (err) return err;
                    params.push_back(pName);
                    paramDefaults.push_back(pDef);

                    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                        parser.advance();
                        auto [rpName, rpDef, rerr] = parser.parseOneParam();
                        if (rerr) return std::make_shared<ErrorNode>("Expected parameter name after ','");
                        params.push_back(rpName);
                        paramDefaults.push_back(rpDef);
                    }

                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                        parser.advance();
                    } else {
                        if (parser.current().type == TokenType::ERROR)
                            return std::make_shared<ErrorNode>(parser.current().value);
                        return std::make_shared<ErrorNode>("Expected ')' in function definition");
                    }
                }

                if (parser.current().type == TokenType::OPERATOR && parser.current().value == "{") {
                    parser.advance();

                    auto [body, hasReturn] = parseFunctionBody(parser);

                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
                        parser.advance();
                        return std::make_shared<FunctionDefNode>(funcName, params, paramDefaults, body, hasReturn);
                    } else {
                        if (parser.current().type == TokenType::ERROR)
                            return std::make_shared<ErrorNode>(parser.current().value);
                        return std::make_shared<ErrorNode>("Expected '}' in function definition");
                    }
                } else {
                    if (parser.current().type == TokenType::ERROR)
                        return std::make_shared<ErrorNode>(parser.current().value);
                    return std::make_shared<ErrorNode>("Expected '{' in function definition");
                }
            }
        }

        // Parse right operand
        auto right = nextHandler->handle();
        if (!right) return std::make_shared<ErrorNode>("Expected right operand for: " + op);

        // Validate assignment target
        if (token.value == "=" && dynamic_cast<IdNode*>(left.get()) == nullptr)
            return std::make_shared<ErrorNode>("Left side of assignment must be an identifier");

        // Create binary operation node
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
        if (token.value == "=") break;  // Assignment is right-associative, break to avoid chaining
    }

    return left;
}

PostfixParser::PostfixParser(Parser& p) : BaseParser(p) {}

std::shared_ptr<ASTNode> PostfixParser::handle() {
    // Parse the base expression via the rest of the chain
    auto expr = nextHandler->handle();
    if (!expr) return expr;

    // Handle postfix operations: expression[index]
    while (parser.current().type == TokenType::OPERATOR && parser.current().value == "[") {
        parser.advance();  // Consume '['
        auto indexExpr = parser.parseExpression();
        if (parser.current().type == TokenType::ERROR)
            return std::make_shared<ErrorNode>(parser.current().value);
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != "]")
            return std::make_shared<ErrorNode>("Expected ']' in index access");
        parser.advance();  // Consume ']'
        expr = std::make_shared<IndexNode>(expr, indexExpr);
    }

    return expr;
}

} // namespace ParserSpace
} // namespace DemoLang
