/**
 * @file include/tokens.hpp
 * @brief The token types used in the lexical analysis.
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

} // namespace Tokens

} // namespace DemoLang

#endif // DEMOLANG_TOKENS