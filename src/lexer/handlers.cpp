/**
 * @file src/lexer/handlers.cpp
 * @brief Handlers for lexer.
 * @author The-XiaoBai
 * @date 2026/01/03
**/

#include "lexer.hpp"
#include "utils.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;


namespace DemoLang {

std::shared_ptr<Token> EOFHandler::handle() {
    // Check if we've reached end of input
    if (lexer.current() == '\0') {
        return std::make_shared<Token>(TokenType::END, "");
    }
    // Pass to next handler if not end of input
    return nextHandler->handle();
};


std::shared_ptr<Token> WhitespaceHandler::handle() {
    // Check if current character is whitespace
    std::string str(1, lexer.current());
    if (std::find(whitespaces.begin(), whitespaces.end(), str) != whitespaces.end()) {
        // Skip whitespace and continue with next character
        lexer.advance();
    }
    // Continue processing (whitespace tokens are not added to token stream)
    return nextHandler->handle();
};


std::shared_ptr<Token> OperatorHandler::handle() {
    char c = lexer.current();
    // Try to match multi-character operators first (longer operators have priority)
    for (const auto& op : operators) {
        if (c == op[0] && lexer.pos() + op.length() <= lexer.getInput().length()) {
            std::string potentialOp = lexer.getInput().substr(lexer.pos(), op.length());
            if (potentialOp == op && std::find(operators.begin(), operators.end(), potentialOp) != operators.end()) {
                // Found matching operator, advance position and return token
                lexer.advance(op.length());
                return std::make_shared<Token>(TokenType::OPERATOR, op);
            }
        }
    }
    // No operator matched, pass to next handler
    return nextHandler->handle();
};


std::shared_ptr<Token> IdentifierHandler::handle() {
    char c = lexer.current();
    // Check if character starts an identifier (letter or underscore)
    if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
        std::string value;
        // Continue collecting alphanumeric characters and underscores
        while (isalnum(lexer.current()) || lexer.current() == '_') {
            value += lexer.current();
            lexer.advance();
        }
        return std::make_shared<Token>(TokenType::IDENTIFIER, value);
    }
    // Not an identifier, pass to next handler
    return nextHandler->handle();
};


std::shared_ptr<Token> NumberHandler::handle() {
    char c = lexer.current();
    // Check if character starts a number
    if (isdigit(static_cast<unsigned char>(c))) {
        std::string value;
        bool hasDecimal = false;
    
        // Process integer part (sequence of digits)
        while (isdigit(static_cast<unsigned char>(lexer.current()))) {
            value += lexer.current();
            lexer.advance();
        }
    
        // Process decimal part if decimal point found
        if (lexer.current() == '.') {
            hasDecimal = true;
            value += lexer.current();
            lexer.advance();
        
            // Must have at least one digit after decimal point
            if (!isdigit(static_cast<unsigned char>(lexer.current()))) 
                return std::make_shared<Token>(TokenType::ERROR, "Invalid float: " + value);
        
            // Process fractional part
            while (isdigit(static_cast<unsigned char>(lexer.current()))) {
                value += lexer.current();
                lexer.advance();
            }
        
            // Check for multiple decimal points
            if (lexer.current() == '.') 
                return std::make_shared<Token>(TokenType::ERROR, "Multiple decimal points");
        }
    
        // Return appropriate token type based on presence of decimal point
        return std::make_shared<Token>(
            hasDecimal ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL,
            value
        );
    }
    // Not a number, pass to next handler
    return nextHandler->handle();
};


std::shared_ptr<Token> StringHandler::handle() {
    char c = lexer.current();
    // Check for string literal (single or double quotes)
    if (c == '\"' || c == '\'') {
        char quote = c;
        std::string value;
        value += quote;
        lexer.advance();
    
        // Collect characters until closing quote or end of input
        while (lexer.current() != quote && lexer.current() != '\0') {
            if (lexer.current() == '\\') {
                // Handle escape sequences
                value += lexer.current();
                lexer.advance();
                if (lexer.current() != '\0') value += lexer.current();
            } else {
                value += lexer.current();
            }
            lexer.advance();
        }
    
        if (lexer.current() == quote) {
            // Found closing quote
            value += quote;
            lexer.advance();
            return std::make_shared<Token>(TokenType::STRING_LITERAL, value);
        } else {
            // Unterminated string literal
            return std::make_shared<Token>(TokenType::ERROR, "Unterminated string: " + value);
        }
    }
    // Not a string literal, pass to next handler
    return nextHandler->handle();
};


std::shared_ptr<Token> UnknownHandler::handle() {
    // Handle any unrecognized character as an error
    std::string value;
    value += lexer.current();
    lexer.advance();
    return std::make_shared<Token>(TokenType::ERROR, "Unknown character: " + value);
};

} // namespace DemoLang