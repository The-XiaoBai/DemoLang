/**
 * @file src/parser/singles.cpp
 * @brief Parser implementation for single tokens.
**/

#include "parser.hpp"
#include "utils.hpp"

namespace DemoLang {

namespace ParserSpace {

/**
 * @brief Flyweight factory for AST nodes
 */
class ASTFlyweight {
private:
    static Utils::FlyweightFactory<std::string, ASTNode>& factory() {
        return Utils::FlyweightFactory<std::string, ASTNode>::instance();
    }
    
public:
    static std::shared_ptr<ASTNode> getIdNode(const std::string& name) {
        return factory().getFlyweight("id:" + name, [&name]() {
            return std::make_shared<IdNode>(name);
        });
    }
    
    static std::shared_ptr<ASTNode> getIntNode(long long value) {
        std::string key = "int:" + std::to_string(value);
        return factory().getFlyweight(key, [value]() {
            return std::make_shared<IntNode>(value);
        });
    }
    
    static std::shared_ptr<ASTNode> getFloatNode(long double value) {
        std::string key = "float:" + std::to_string(value);
        return factory().getFlyweight(key, [value]() {
            return std::make_shared<FloatNode>(value);
        });
    }
    
    static std::shared_ptr<ASTNode> getStringNode(const std::string& value) {
        std::string key = "string:" + value;
        return factory().getFlyweight(key, [&value]() {
            return std::make_shared<StringNode>(value);
        });
    }
    
    static std::shared_ptr<ASTNode> getFunctionCallNode(const std::string& name, const std::vector<std::shared_ptr<ASTNode>>& args) {
        return std::make_shared<FunctionCallNode>(name, args);
    }
    
    static std::shared_ptr<ASTNode> getFunctionDefNode(const std::string& name, const std::vector<std::string>& params, const std::vector<std::shared_ptr<ASTNode>>& defaults, std::shared_ptr<ASTNode> body) {
        return std::make_shared<FunctionDefNode>(name, params, defaults, body);
    }

    static std::shared_ptr<ASTNode> getLambdaNode(const std::vector<std::string>& params, const std::vector<std::shared_ptr<ASTNode>>& defaults, std::shared_ptr<ASTNode> body) {
        return std::make_shared<LambdaNode>(params, defaults, body);
    }

    static std::shared_ptr<ASTNode> createParenthesizedNode(const Token& token, ParserSpace::Parser& parser, bool consumeOpenParen = true) {
        // Advance past the opening parenthesis if needed
        if (consumeOpenParen) {
            parser.advance();
        }

        // Check if we've reached end of input prematurely
        if (parser.current().type == TokenType::END) {
            return std::make_shared<ErrorNode>("Unexpected end of input, expected closing parenthesis");
        }

        // Check if this is a lambda function: (params){body}
        bool isLambda = false;
        std::vector<std::string> lambdaParams;
        std::vector<std::shared_ptr<ASTNode>> paramDefaults;
        
        // Try to parse parameters
        if (parser.current().type == TokenType::IDENTIFIER) {
            isLambda = true;
            
            // Parse first parameter
            std::string paramName = parser.current().value;
            parser.advance(); // Consume parameter name
            
            // Check for default value
            std::shared_ptr<ASTNode> defaultValue = nullptr;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "=") {
                parser.advance(); // Consume '='
                defaultValue = parser.parseExpression();
            }
            lambdaParams.push_back(paramName);
            paramDefaults.push_back(defaultValue);
            
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
                    lambdaParams.push_back(paramName);
                    paramDefaults.push_back(defaultValue);
                } else {
                    return std::make_shared<ErrorNode>("Expected parameter name after ','");
                }
            }
            
            // Expect closing ')' for parameters
            if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")") {
                return std::make_shared<ErrorNode>("Expected ')' after parameters");
            }
            parser.advance(); // Consume ')'
            
            // Expect '{' for lambda body
            if (parser.current().type != TokenType::OPERATOR || parser.current().value != "{") {
                return std::make_shared<ErrorNode>("Expected '{' for lambda body");
            }
            parser.advance(); // Consume '{'
            
            // Parse lambda body with @ return
            std::shared_ptr<ASTNode> body;
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "@") {
                parser.advance(); // Consume '@'
                body = parser.parseExpression();
            } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "}") {
                // Empty lambda body - returns empty
                body = std::make_shared<StringNode>("");
            } else {
                // Parse expression as statement (no return value)
                body = parser.parseExpression();
            }
            
            // Expect closing '}'
            if (parser.current().type != TokenType::OPERATOR || parser.current().value != "}") {
                return std::make_shared<ErrorNode>("Expected '}' for lambda body");
            }
            parser.advance(); // Consume '}'
            
            // Create lambda node
            return getLambdaNode(lambdaParams, paramDefaults, body);
        }
        
        // Not a lambda, parse as regular parenthesized expression
        auto expr = parser.parseExpression();

        // Expect closing parenthesis
        if (parser.current().type != TokenType::OPERATOR || parser.current().value != ")") {
            return std::make_shared<ErrorNode>("Expected closing parenthesis");
        }

        // Advance past the closing parenthesis
        parser.advance();

        return expr;
    }

    static void clearCache() { factory().clear(); }
    static size_t cacheSize() { return factory().size(); }
};

// AST Node Factory using Utils Factory template
class ASTNodeFactory : public Utils::Factory<TokenType, ASTNode>, public Utils::Singleton<ASTNodeFactory> {
private:
    ASTNodeFactory() = default;
    friend class Utils::Singleton<ASTNodeFactory>;
    
    void initializeCreators() {
        if (!isInitialized()) {
            registerCreator(TokenType::STRING_LITERAL, []() { return std::make_unique<StringNode>(""); });
            registerCreator(TokenType::INTEGER_LITERAL, []() { return std::make_unique<IntNode>(0); });
            registerCreator(TokenType::FLOAT_LITERAL, []() { return std::make_unique<FloatNode>(0.0); });
            registerCreator(TokenType::IDENTIFIER, []() { return std::make_unique<IdNode>(""); });
            registerCreator(TokenType::OPERATOR, []() { return std::make_unique<ErrorNode>("Unexpected operator"); });
            registerCreator(TokenType::ERROR, []() { return std::make_unique<ErrorNode>("Error"); });
        }
    }

public:
    std::shared_ptr<ASTNode> createNode(const Token& token, ParserSpace::Parser& parser) {
        initializeCreators();
        
        switch (token.type) {
            case TokenType::STRING_LITERAL:
                return createStringNode(token);
            case TokenType::INTEGER_LITERAL:
                return createIntNode(token);
            case TokenType::FLOAT_LITERAL:
                return createFloatNode(token);
            case TokenType::IDENTIFIER:
                return ASTFlyweight::getIdNode(token.value);
            case TokenType::OPERATOR:
                if (token.value == "(") {
                    return ASTFlyweight::createParenthesizedNode(token, parser);
                }
                return std::make_shared<ErrorNode>("Unexpected operator: " + token.value);
            case TokenType::ERROR:
                return createErrorNode(token.value);
            default:
                return std::make_shared<ErrorNode>("Unexpected token: " + token.value);
        }
    }
    
    std::shared_ptr<ASTNode> createErrorNode(const std::string& message) {
        return std::make_shared<ErrorNode>(message);
    }
    
private:
    std::shared_ptr<ASTNode> createStringNode(const Token& token) {
        if (token.value.size() >= 2) {
            char first = token.value.front(), last = token.value.back();
            if ((first == '\"' && last == '\"') || (first == '\'' && last == '\''))
                return ASTFlyweight::getStringNode(token.value.substr(1, token.value.size() - 2));
        }
        return std::make_shared<ErrorNode>("Invalid string: " + token.value);
    }
    
    std::shared_ptr<ASTNode> createIntNode(const Token& token) {
        try {
            long long val = std::stoll(token.value);
            return ASTFlyweight::getIntNode(static_cast<long long>(val));
        } catch (...) { 
            return std::make_shared<ErrorNode>("Invalid integer: " + token.value); 
        }
    }
    
    std::shared_ptr<ASTNode> createFloatNode(const Token& token) {
        try {
            char* end;
            double val = std::strtold(token.value.c_str(), &end);
            if (end != token.value.c_str() + token.value.length() || errno == ERANGE)
                return std::make_shared<ErrorNode>("Invalid float");
            return ASTFlyweight::getFloatNode(static_cast<long double>(val));
        } catch (...) {
            return std::make_shared<ErrorNode>("Invalid float: " + token.value);
        }
    }
};

std::shared_ptr<ASTNode> ParserSpace::PrimaryParser::handle() {
    Token token = parser.current();
    
    // Handle function calls: identifier followed by '('
    if (token.type == TokenType::IDENTIFIER) {
        std::string funcName = token.value;
        parser.advance(); // Consume identifier

        // Check if next token is '(' for function call
        if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
            parser.advance(); // Consume '('

            // Parse arguments
            std::vector<std::shared_ptr<ASTNode>> args;

            // Handle empty argument list
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance(); // Consume ')'
                return ASTFlyweight::getFunctionCallNode(funcName, args);
            }

            // Parse first argument (support a=1 syntax and lambda)
            if (parser.current().type == TokenType::IDENTIFIER) {
                std::string paramName = parser.current().value;
                parser.advance(); // Consume parameter name
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == "=") {
                    parser.advance(); // Consume '='
                    args.push_back(parser.parseExpression());
                } else {
                    // Not a named parameter, treat as expression
                    args.push_back(ASTFlyweight::getIdNode(paramName));
                }
            } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                // Handle lambda as argument: (x){x}()
                args.push_back(ASTFlyweight::createParenthesizedNode(parser.current(), parser, false));
            } else {
                args.push_back(parser.parseExpression());
            }

            // Parse remaining arguments
            while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                parser.advance(); // Consume ','
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    return std::make_shared<ErrorNode>("Unexpected ',' before ')'");
                }
                // Support a=1 syntax for named parameters and lambda
                if (parser.current().type == TokenType::IDENTIFIER) {
                    std::string paramName = parser.current().value;
                    parser.advance(); // Consume parameter name
                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == "=") {
                        parser.advance(); // Consume '='
                        args.push_back(parser.parseExpression());
                    } else {
                        // Not a named parameter, treat as expression
                        args.push_back(ASTFlyweight::getIdNode(paramName));
                    }
                } else if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                    // Handle lambda as argument: (x){x}()
                    args.push_back(ASTFlyweight::createParenthesizedNode(parser.current(), parser, false));
                } else {
                    args.push_back(parser.parseExpression());
                }
            }

            // Expect closing ')'
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                parser.advance(); // Consume ')'
                return ASTFlyweight::getFunctionCallNode(funcName, args);
            } else {
                return std::make_shared<ErrorNode>("Expected ')' in function call");
            }
        }

        // Not a function call, return identifier
        return ASTFlyweight::getIdNode(funcName);
    }

    // Handle lambda call: (params){body}(args)
    if (token.type == TokenType::OPERATOR && token.value == "(") {
        auto lambda = ASTFlyweight::createParenthesizedNode(token, parser);
        if (dynamic_cast<LambdaNode*>(lambda.get())) {
            // Lambda followed by arguments for immediate execution
            if (parser.current().type == TokenType::OPERATOR && parser.current().value == "(") {
                parser.advance(); // Consume '('

                // Parse arguments
                std::vector<std::shared_ptr<ASTNode>> args;

                // Handle empty argument list
                if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                    parser.advance(); // Consume ')'
                } else {
                    args.push_back(parser.parseExpression());

                    // Parse remaining arguments
                    while (parser.current().type == TokenType::OPERATOR && parser.current().value == ",") {
                        parser.advance(); // Consume ','
                        args.push_back(parser.parseExpression());
                    }

                    // Expect closing ')'
                    if (parser.current().type == TokenType::OPERATOR && parser.current().value == ")") {
                        parser.advance(); // Consume ')'
                    } else {
                        return std::make_shared<ErrorNode>("Expected ')' in lambda call");
                    }
                }

                // Create function call with lambda as function
                // Store the lambda node directly in the FunctionCallNode for immediate execution
                return std::make_shared<FunctionCallNode>(lambda, args);
            }
        }
        return lambda;
    }
    
    // Don't advance here for '(' tokens, let createParenthesizedNode handle it
    if (token.type == TokenType::OPERATOR && token.value == "(") {
        return ASTNodeFactory::instance().createNode(token, parser);
    }
    
    parser.advance();
    return ASTNodeFactory::instance().createNode(token, parser);
}

} // namespace ParserSpace

} // namespace DemoLang