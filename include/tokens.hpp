/**
 * @file include/tokens.hpp
 * @brief The token types used in the lexical analysis.
 * @author The-XiaoBai
 * @date 2026/01/31
**/

#pragma once
#ifndef DEMOLANG_TOKENS
#define DEMOLANG_TOKENS

#include <string>
#include <vector>
#include "utils.hpp"


namespace DemoLang {

namespace Tokens {

/**
 * @brief Token types used in the lexical analysis.
**/
enum class TokenType {
    END,                // End of input
    OPERATOR,           // Plus, minus, multiply, divide, etc.
    IDENTIFIER,         // Variable names, function names, etc.
    INTEGER_LITERAL,    // Integer
    FLOAT_LITERAL,      // Float
    STRING_LITERAL,     // String
    ERROR               // Error
};


/**
 * @brief List of supported whitespaces.
**/
const std::vector<std::string> whitespaces = {
    " ", "\t", "\n", "\r"
};


/**
 * @brief List of supported operators.
**/
const std::vector<std::string> operators = {
    "==", "!=", ">=", "<=", ">", "<",
    "=", "(", ")", "{", "}",
    "+", "-", "*", "/",
    "!", "&", "|"
};


/**
 * @brief Token structure representing a lexical unit.
**/
struct Token {
    TokenType type;
    std::string value;
    Token(TokenType type, const std::string &value) : type(type), value(value) {}
};


/**
 * @brief Flyweight factory for tokens
 */
class TokenFlyweight {
private:
    static Utils::FlyweightFactory<std::string, Token>& factory() {
        return Utils::FlyweightFactory<std::string, Token>::instance();
    }
    
public:
    static std::shared_ptr<Token> getToken(TokenType type, const std::string& value = "") {
        auto key = std::to_string(static_cast<int>(type)) + ":" + value;
        return factory().getFlyweight(key, [type, &value]() {
            return std::make_shared<Token>(type, value);
        });
    }
    
    static void clearCache() { factory().clear(); }
    static size_t cacheSize() { return factory().size(); }
};

} // namespace Tokens

} // namespace DemoLang

#endif // DEMOLANG_TOKENS