/**
 * @file src/include/lexer.hpp
 * @brief Tokenizing input to tokens.
**/

#pragma once
#ifndef DEMOLANG_LEXER
#define DEMOLANG_LEXER

#include "tokens.hpp"
#include "utils.hpp"
#include <vector>
#include <memory>

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;


namespace DemoLang {

namespace LexerSpace {

/**
 * @brief Lexer class for tokenizing input.
**/
class Lexer : public Singleton<Lexer> {
    friend class Singleton<Lexer>;

private:
    std::string input;
    size_t position;

public:
    Lexer();
    
    std::string getInput() const;
    size_t pos() const;
    char current() const;
    void advance(size_t step=1);
    Token nextToken();
    std::vector<Token> tokenize(const std::string &input);
};


/**
 * @brief Base handler for token recognition.
**/
class BaseHandler : public Handler<Token> {
protected:
    Lexer& lexer;

public:
    BaseHandler(Lexer& lexer);
    virtual std::shared_ptr<Token> handle() = 0;
};


/**
 * @brief Handler for end of file tokens.
**/
class EOFHandler : public BaseHandler {
public:
    EOFHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for whitespace tokens.
**/
class WhitespaceHandler : public BaseHandler {
public:
    WhitespaceHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for operator tokens.
**/
class OperatorHandler : public BaseHandler {
public:
    OperatorHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for identifier tokens.
**/
class IdentifierHandler : public BaseHandler {
public:
    IdentifierHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for number tokens.
**/
class NumberHandler : public BaseHandler {
public:
    NumberHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for string tokens.
**/
class StringHandler : public BaseHandler {
public:
    StringHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};


/**
 * @brief Handler for unknown/error tokens.
**/
class UnknownHandler : public BaseHandler {
public:
    UnknownHandler(Lexer& lexer);
    std::shared_ptr<Token> handle();
};

} // namespace LexerSpace

} // namespace DemoLang

#endif // DEMOLANG_LEXER