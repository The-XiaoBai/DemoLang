/**
 * @file include/parser.hpp
 * @brief Parse tokens into an Abstract Syntax Tree (AST).
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#pragma once
#ifndef DEMOLANG_PARSER
#define DEMOLANG_PARSER

#include "tokens.hpp"
#include "lexer.hpp"
#include "ast.hpp"
#include <memory>
#include <vector>

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;
using namespace DemoLang::AST;


namespace DemoLang {

namespace ParserSpace {

/**
 * @brief Parser class for converting tokens to AST.
**/
class Parser : public Singleton<Parser> {
    friend class Singleton<Parser>;

private:
    std::vector<Token> tokens;
    size_t current_pos;

public:
    Parser() : current_pos(0) {};
    
    Token current() const { return current_pos < tokens.size() ? tokens[current_pos] : Token(TokenType::END, ""); }
    void advance() { if (current_pos < tokens.size()) current_pos++; }
    bool match(TokenType type, const std::string& value);
    std::shared_ptr<ASTNode> parse(const std::vector<Token> &tokens);
};


class BaseParser : public Handler<ASTNode> {
protected:
    Parser& parser;

public:
    BaseParser(Parser& parser) : parser(parser) {};
    virtual std::shared_ptr<ASTNode> handle() = 0;
};


class AParser : public BaseParser {
public:
    AParser(Parser& parser) : BaseParser(parser) {};
    std::shared_ptr<ASTNode> handle();
};

} // namespace ParserSpace

} // namespace DemoLang

#endif // DEMOLANG_PARSER