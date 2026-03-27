/**
 * @file src/parser/operators.cpp
 * @brief Parser implementation for operators.
**/

#include "parser.hpp"
#include "utils.hpp"

// Forward declaration of ASTFlyweight
class ASTFlyweight {
public:
    static std::shared_ptr<ASTNode> getFunctionDefNode(const std::string& name, const std::vector<std::string>& params, const std::vector<std::shared_ptr<ASTNode>>& defaults, std::shared_ptr<ASTNode> body);
    static std::shared_ptr<ASTNode> getReturnNode(std::shared_ptr<ASTNode> value);
};

// Implementation of ASTFlyweight::getFunctionDefNode
std::shared_ptr<ASTNode> ASTFlyweight::getFunctionDefNode(const std::string& name, const std::vector<std::string>& params, const std::vector<std::shared_ptr<ASTNode>>& defaults, std::shared_ptr<ASTNode> body) {
    return std::make_shared<FunctionDefNode>(name, params, defaults, body);
}

// Implementation of ASTFlyweight::getReturnNode
std::shared_ptr<ASTNode> ASTFlyweight::getReturnNode(std::shared_ptr<ASTNode> value) {
    return std::make_shared<ReturnNode>(value);
}


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
        
        // Check for function definition: identifier = (params) { body }
        if (op == "=" && dynamic_cast<IdNode*>(left.get()) != nullptr) {
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                std::string funcName = dynamic_cast<IdNode*>(left.get())->getName();
                parser.advance(); // Consume '('
                
                // Parse parameters
                std::vector<std::string> params;
                std::vector<std::shared_ptr<ASTNode>> paramDefaults;

                // Handle empty parameter list
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    parser.advance(); // Consume ')'
                } else {
                    // Parse first parameter (support default value: a=1)
                    if (parser.current().type == TokenType::IDENTIFIER) {
                        std::string paramName = parser.current().value;
                        parser.advance(); // Consume parameter name
                        // Check for default value
                        std::shared_ptr<ASTNode> defaultValue = nullptr;
                        if (parser.current().type == TokenType::OPERATOR && parser.current().value == "=") {
                            parser.advance(); // Consume '='
                            defaultValue = parser.parseExpression();
                        }
                        params.push_back(paramName);
                        paramDefaults.push_back(defaultValue);
                    } else {
                        return std::make_shared<ErrorNode>("Expected parameter name");
                    }

                    // Parse remaining parameters
                    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                        parser.advance(); // Consume ','
                        if (parser.current().type == TokenType::IDENTIFIER) {
                            std::string paramName = parser.current().value;
                            parser.advance(); // Consume parameter name
                            // Check for default value
                            std::shared_ptr<ASTNode> defaultValue = nullptr;
                            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "=") {
                                parser.advance(); // Consume '='
                                defaultValue = parser.parseExpression();
                            }
                            params.push_back(paramName);
                            paramDefaults.push_back(defaultValue);
                        } else {
                            return std::make_shared<ErrorNode>("Expected parameter name after ','");
                        }
                    }

                    // Expect closing ')'
                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                        parser.advance(); // Consume ')'
                    } else {
                        return std::make_shared<ErrorNode>("Expected ')' in function definition");
                    }
                }
                
                // Expect '{' for function body
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == "{") {
                    parser.advance(); // Consume '{'
                    
                    // Check for return value syntax @value
                    std::shared_ptr<ASTNode> body;
                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "@") {
                        parser.advance(); // Consume '@'
                        auto returnValue = parser.parseExpression();
                        body = ASTFlyweight::getReturnNode(returnValue);
                    } else {
                        body = parser.parseExpression();
                    }
                    
                    // Expect closing '}'
                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
                        parser.advance(); // Consume '}'
                        return ASTFlyweight::getFunctionDefNode(funcName, params, paramDefaults, body);
                    } else {
                        return std::make_shared<ErrorNode>("Expected '}' in function definition");
                    }
                } else {
                    return std::make_shared<ErrorNode>("Expected '{' in function definition");
                }
            }
        }
        
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