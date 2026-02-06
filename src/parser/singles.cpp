/**
 * @file src/parser/singles.cpp
 * @brief Parser implementation for single tokens.
 * @author The-XiaoBai
 * @date 2026/01/31
**/

#include "parser.hpp"
#include "utils.hpp"

namespace DemoLang {

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
                    return createParenthesizedNode(token, parser);
                }
                return std::make_shared<ErrorNode>("Unexpected operator: " + token.value);
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
    
    std::shared_ptr<ASTNode> createParenthesizedNode(const Token& token, ParserSpace::Parser& parser) {
        parser.advance();
        auto expr = parser.parseExpression();
        if (!expr) return std::make_shared<ErrorNode>("Empty expression in parentheses");        
        if (!parser.match(TokenType::OPERATOR, ")"))
            return std::make_shared<ErrorNode>("Expected closing parenthesis");
        return expr;
    }
};

std::shared_ptr<ASTNode> ParserSpace::PrimaryParser::handle() {
    Token token = parser.current();
    parser.advance();
    return ASTNodeFactory::instance().createNode(token, parser);
}

} // namespace DemoLang