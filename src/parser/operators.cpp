/**
 * @file src/parser/operators.cpp
 * @brief Parser implementation for operators.
**/

#include "parser.hpp"
#include "utils.hpp"


namespace DemoLang {

std::shared_ptr<ASTNode> ParserSpace::UnaryParser::handle() {
    Token token = parser.current();
    // Check if current token is a unary operator we handle
    if (token.type == TokenType::OPERATOR && std::find(operators.begin(), operators.end(), token.value) != operators.end()) {
        parser.advance(); // Consume the operator
        auto operand = nextHandler->handle(); // Parse the operand (right-associative)
        if (!operand) return std::make_shared<ErrorNode>("Expected expression after: " + token.value);
        return std::make_shared<UnaryOpNode>(token.value, std::move(operand));
    }
    // Not a unary operator, pass to next handler
    return nextHandler->handle();
}


std::shared_ptr<ASTNode> ParserSpace::BinaryParser::handle() {
    auto left = nextHandler->handle();  // Parse left operand first
    if (!left) return std::make_shared<ErrorNode>("Left part can not be parsed");
    
    // Process a chain of binary operations with same precedence (left-associative)
    while (true) {
        Token token = parser.current();
        // Check if current token is a binary operator we handle
        if (token.type != TokenType::OPERATOR
            || std::find(operators.begin(), operators.end(), token.value) == operators.end())
            break;
        
        std::string op = token.value;
        parser.advance(); // Consume the operator
        auto right = nextHandler->handle();  // Parse right operand
        if (!right) return std::make_shared<ErrorNode>("Expected right operand for: " + op);

        // Special validation for assignment operator
        if (token.value == "=" && dynamic_cast<IdNode*>(left.get()) == nullptr)
            return std::make_shared<ErrorNode>("Left side of assignment must be an identifier");
        
        // Create binary operation node and continue for chaining
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
        if (token.value == "=") break;  // Assignment is right-associative and doesn't chain
    }
    
    return left;
}

} // namespace DemoLang