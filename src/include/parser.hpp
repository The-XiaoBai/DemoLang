/**
 * @file src/include/parser.hpp
 * @brief Parse tokens into an Abstract Syntax Tree (AST).
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
    Parser();
    
    Token current() const;
    void advance();
    bool match(TokenType type, const std::string& value);
    std::shared_ptr<ASTNode> parse(const std::vector<Token> &tokens);
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseExpressionInternal();
};


/**
 * @brief Base parser for AST node generation.
**/
class BaseParser : public Handler<ASTNode> {
protected:
    Parser& parser;

public:
    BaseParser(Parser& parser);
    virtual std::shared_ptr<ASTNode> handle() = 0;
};


/**
 * @brief Parser for unary operations.
**/
class UnaryParser : public BaseParser {
private:
    std::vector<std::string> operators;

public:
    UnaryParser(Parser& parser, std::vector<std::string> operators);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for binary operations.
**/
class BinaryParser : public BaseParser {
private:
    std::vector<std::string> operators;

public:
    BinaryParser(Parser& parser, std::vector<std::string> operators);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for primary expressions.
**/
class PrimaryParser : public BaseParser {
public:
    PrimaryParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};

} // namespace ParserSpace

} // namespace DemoLang

#endif // DEMOLANG_PARSER