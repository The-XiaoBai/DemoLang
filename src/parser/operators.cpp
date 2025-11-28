/**
 * @file src/parser/operators.cpp
 * @brief Parser implementation for operators.
 * @author The-XiaoBai
 * @date 2025/11/16
**/

#include "parser.hpp"
#include "utils.hpp"


namespace DemoLang {

std::shared_ptr<ASTNode> ParserSpace::UnaryParser::handle() {
    Token token = parser.current();
    if (token.type == TokenType::OPERATOR && std::find(operators.begin(), operators.end(), token.value) != operators.end()) {
        parser.advance();
        auto operand = nextHandler->handle();
        if (!operand) return std::make_shared<ErrorNode>("Expected expression after: " + token.value);
        return std::make_shared<UnaryOpNode>(token.value, std::move(operand));
    }
    return nextHandler->handle();
}


std::shared_ptr<ASTNode> ParserSpace::BinaryParser::handle() {
    auto left = nextHandler->handle();  // Parse left operand
    if (!left) return std::make_shared<ErrorNode>("Left part can not be parsed");
    
    // Process a chain of binary operations with same precedence
    while (true) {
        Token token = parser.current();
        if (token.type != TokenType::OPERATOR
            || std::find(operators.begin(), operators.end(), token.value) == operators.end())
            break;
        
        std::string op = token.value;
        parser.advance();
        auto right = nextHandler->handle();  // Parse right operand
        if (!right) return std::make_shared<ErrorNode>("Expected right operand for: " + op);

        // Assignment requires identifier on left side
        if (token.value == "=" && dynamic_cast<IdNode*>(left.get()) == nullptr)
            return std::make_shared<ErrorNode>("Left side of assignment must be an identifier");
        
        left = std::make_shared<BinaryOpNode>(op, std::move(left), std::move(right));
        if (token.value == "=") break;  // Assignment doesn't chain
    }
    
    return left;
}

} // namespace DemoLang