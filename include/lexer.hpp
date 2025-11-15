/**
 * @file include/lexer.hpp
 * @brief Tokenizing input to tokens.
 * @author The-XiaoBai
 * @date 2025/11/15
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
    Lexer() : position(0) {};
    
    std::string getInput() const { return input; };
    size_t pos() const { return position; };
    char current() const { return position >= input.length() ? '\0' : input[position]; };
    void advance(size_t step=1) { position += step; };
    Token nextToken();
    std::vector<Token> tokenize(const std::string &input);
};


class BaseHandler : public Handler<Token> {
protected:
    Lexer& lexer;

public:
    BaseHandler(Lexer& lexer) : lexer(lexer) {};
    virtual std::shared_ptr<Token> handle() = 0;
};


class EOFHandler : public BaseHandler {
public:
    EOFHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class WhitespaceHandler : public BaseHandler {
public:
    WhitespaceHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class OperatorHandler : public BaseHandler {
public:
    OperatorHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class KeywordHandler : public BaseHandler {
public:
    KeywordHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class IdentifierHandler : public BaseHandler {
public:
    IdentifierHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class NumberHandler : public BaseHandler {
public:
    NumberHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class StringHandler : public BaseHandler {
public:
    StringHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};


class UnknownHandler : public BaseHandler {
public:
    UnknownHandler(Lexer& lexer) : BaseHandler(lexer) {}
    std::shared_ptr<Token> handle();
};

} // namespace LexerSpace

} // namespace DemoLang

#endif // DEMOLANG_LEXER