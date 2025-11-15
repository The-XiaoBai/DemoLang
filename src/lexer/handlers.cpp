/**
 * @file src/lexer/handlers.cpp
 * @brief Handlers for lexer.
 * @author The-XiaoBai
 * @date 2025/11/15
**/

#include "lexer.hpp"
#include "utils.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;
using namespace DemoLang::LexerSpace;


namespace DemoLang {

std::shared_ptr<Token> EOFHandler::handle() {
    if (lexer.current() == '\0') {
        return std::make_shared<Token>(TokenType::END, "");
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> WhitespaceHandler::handle() {
    std::string str(1, lexer.current());
    if (std::find(whitespaces.begin(), whitespaces.end(), str) != whitespaces.end()) {
        lexer.advance();
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> OperatorHandler::handle() {
    char c = lexer.current();
    for (const auto& op : operators) {
        if (c == op[0] && lexer.pos() + op.length() <= lexer.getInput().length()) {
            std::string potentialOp = lexer.getInput().substr(lexer.pos(), op.length());
            if (potentialOp == op && std::find(operators.begin(), operators.end(), potentialOp) != operators.end()) {
                lexer.advance(op.length());
                return std::make_shared<Token>(TokenType::OPERATOR, op);
            }
        }
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> KeywordHandler::handle() {
    char c = lexer.current();
    for (const auto& kw : keywords) {
        if (c == kw[0] && lexer.pos() + kw.length() <= lexer.getInput().length()) {
            std::string potentialKw = lexer.getInput().substr(lexer.pos(), kw.length());
            if (potentialKw == kw && std::find(keywords.begin(), keywords.end(), potentialKw) != keywords.end()) {
                lexer.advance(kw.length());
                return std::make_shared<Token>(TokenType::KEYWORD, kw);
            }
        }
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> IdentifierHandler::handle() {
    char c = lexer.current();
    // Only support ASCII characters
    if (isalpha(static_cast<unsigned char>(c)) || c == '_') {
        std::string value;
        while (isalnum(lexer.current()) || lexer.current() == '_') {
            value += lexer.current();
            lexer.advance();
        }
        return std::make_shared<Token>(TokenType::IDENTIFIER, value);
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> NumberHandler::handle() {
    char c = lexer.current();
    if (isdigit(static_cast<unsigned char>(c))) {
        std::string value;
        bool hasDecimal = false;
    
        // Process integer part
        while (isdigit(static_cast<unsigned char>(lexer.current()))) {
            value += lexer.current();
            lexer.advance();
        }
    
        // Process decimal part if present
        if (lexer.current() == '.') {
            hasDecimal = true;
            value += lexer.current();
            lexer.advance();
        
            if (!isdigit(static_cast<unsigned char>(lexer.current()))) 
                return std::make_shared<Token>(TokenType::ERROR, "Invalid float: " + value);
        
            while (isdigit(static_cast<unsigned char>(lexer.current()))) {
                value += lexer.current();
                lexer.advance();
            }
        
            if (lexer.current() == '.') 
                return std::make_shared<Token>(TokenType::ERROR, "Multiple decimal points");
        }
    
        return std::make_shared<Token>(
            hasDecimal ? TokenType::FLOAT_LITERAL : TokenType::INTEGER_LITERAL,
            value
        );
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> StringHandler::handle() {
    char c = lexer.current();
    if (c == '\"' || c == '\'') {
        char quote = c;
        std::string value;
        value += quote;
        lexer.advance();
    
        while (lexer.current() != quote && lexer.current() != '\0') {
            if (lexer.current() == '\\') {
                value += lexer.current();
                lexer.advance();
                if (lexer.current() != '\0') value += lexer.current();
            } else {
                value += lexer.current();
            }
            lexer.advance();
        }
    
        if (lexer.current() == quote) {
            value += quote;
            lexer.advance();
            return std::make_shared<Token>(TokenType::STRING_LITERAL, value);
        } else {
            return std::make_shared<Token>(TokenType::ERROR, "Unterminated string: " + value);
        }
    }
    return nextHandler->handle();
};


std::shared_ptr<Token> UnknownHandler::handle() {
    std::string value;
    value += lexer.current();
    lexer.advance();
    return std::make_shared<Token>(TokenType::ERROR, "Unknown character: " + value);
};

} // namespace DemoLang