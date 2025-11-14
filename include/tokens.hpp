/**
 * @file include/tokens.hpp
 * @brief The token types used in the lexical analysis.
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#pragma once
#ifndef DEMOLANG_TOKENS
#define DEMOLANG_TOKENS

#include <string>
#include <vector>


namespace DemoLang {

namespace Tokens {

/**
 * @brief Token types used in the lexical analysis.
**/
enum class TokenType {
    END
};


/**
 * @brief List of supported operators.
**/
const std::vector<std::string> operators = {};


/**
 * @brief List of supported keywords.
**/
const std::vector<std::string> keywords = {};


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