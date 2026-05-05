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
#include <tuple>
#include <utility>


namespace DemoLang {

namespace ParserSpace {

using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;
using namespace DemoLang::AST;


/**
 * @brief Parser class for converting tokens to AST.
**/
class Parser : public Singleton<Parser> {
    friend class Singleton<Parser>;

private:
    std::vector<Token> tokens;
    size_t current_pos;
    Utils::Chain<ASTNode> exprChain;

public:
    Parser();
    
    Token current() const;
    void advance();
    bool match(TokenType type, const std::string& value);
    size_t savePosition() const;
    void restorePosition(size_t pos);
    std::shared_ptr<ASTNode> parse(const std::vector<Token> &tokens);
    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parseExpressionInternal();

    /**
     * @brief Parse a parameter with optional default value.
     * @return (paramName, defaultValue, errorNode) - errorNode is non-null on error.
    **/
    std::tuple<std::string, std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
    parseOneParam();

    /**
     * @brief Parse a function call argument.
     * @return (argNode, errorNode) - errorNode is non-null on error.
    **/
    std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
    parseOneArg();
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
 * @brief Parser for list literals.
**/
class ListParser : public BaseParser {
public:
    ListParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for function calls.
**/
class FunctionCallParser : public BaseParser {
public:
    FunctionCallParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for identifiers and index access.
**/
class IdentifierParser : public BaseParser {
public:
    IdentifierParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for if statements.
**/
class IfParser : public BaseParser {
public:
    IfParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for while statements.
**/
class WhileParser : public BaseParser {
public:
    WhileParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for break/continue statements.
**/
class LoopControlParser : public BaseParser {
public:
    LoopControlParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Parser for parenthesized expressions and lambdas.
**/
class ParenthesizedParser : public BaseParser {
public:
    ParenthesizedParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


/**
 * @brief Fallback parser for literal values.
**/
class LiteralFallbackParser : public BaseParser {
public:
    LiteralFallbackParser(Parser& parser);
    std::shared_ptr<ASTNode> handle() override;
};


} // namespace ParserSpace

} // namespace DemoLang

#endif // DEMOLANG_PARSER