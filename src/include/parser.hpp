/**
 * @file src/include/parser.hpp
 * @brief Parse tokens into an Abstract Syntax Tree (AST).
**/

#pragma once
#ifndef DEMOLANG_PARSER
#define DEMOLANG_PARSER

#include <memory>
#include <tuple>
#include <utility>
#include <vector>
#include "tokens.hpp"
#include "lexer.hpp"
#include "ast.hpp"

namespace DemoLang {

namespace ParserSpace {

/**
 * @brief Parser class for converting tokens to AST.
**/
class Parser : public Utils::Singleton<Parser> {
    friend class Utils::Singleton<Parser>;

private:
    std::vector<Tokens::Token> tokens;
    size_t current_pos;
    Utils::Chain<AST::ASTNode> exprChain;

public:
    Parser();
    
    Tokens::Token current() const;
    void advance();
    bool match(Tokens::TokenType type, const std::string& value);
    size_t savePosition() const;
    void restorePosition(size_t pos);
    std::shared_ptr<AST::ASTNode> parse(const std::vector<Tokens::Token> &tokens);
    std::shared_ptr<AST::ASTNode> parseExpression(bool allowSequencing = true);

    /**
     * @brief Parse a parameter with optional default value.
     * @return (paramName, defaultValue, errorNode) - errorNode is non-null on error.
    **/
    std::tuple<std::string, std::shared_ptr<AST::ASTNode>, std::shared_ptr<AST::ASTNode>>
    parseOneParam();
};

/**
 * @brief Parse the body content of a function/lambda (between { and }).
 *        Does NOT consume the opening '{' or closing '}'.
 * @return (bodyNode, hasReturn)
**/
std::pair<std::shared_ptr<AST::ASTNode>, bool> parseFunctionBody(Parser& parser);

/**
 * @brief Base parser for AST node generation.
**/
class BaseParser : public Utils::Handler<AST::ASTNode> {
protected:
    Parser& parser;

public:
    BaseParser(Parser& parser);
    virtual std::shared_ptr<AST::ASTNode> handle() = 0;
};

/**
 * @brief Parser for unary operations.
**/
class UnaryParser : public BaseParser {
private:
    std::vector<std::string> operators;

public:
    UnaryParser(Parser& parser, std::vector<std::string> operators);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for binary operations.
**/
class BinaryParser : public BaseParser {
private:
    std::vector<std::string> operators;

public:
    BinaryParser(Parser& parser, std::vector<std::string> operators);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for postfix operations (e.g., index access expr[index]).
 *        Applies to any expression parsed by subsequent handlers.
**/
class PostfixParser : public BaseParser {
public:
    PostfixParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for list literals.
**/
class ListParser : public BaseParser {
public:
    ListParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for function calls.
**/
class FunctionCallParser : public BaseParser {
public:
    FunctionCallParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for identifiers and index access.
**/
class IdentifierParser : public BaseParser {
public:
    IdentifierParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for if statements.
**/
class IfParser : public BaseParser {
public:
    IfParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for while statements.
**/
class WhileParser : public BaseParser {
public:
    WhileParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for break/continue statements.
**/
class LoopControlParser : public BaseParser {
public:
    LoopControlParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Parser for parenthesized expressions and lambdas.
**/
class ParenthesizedParser : public BaseParser {
public:
    ParenthesizedParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

/**
 * @brief Fallback parser for literal values.
**/
class LiteralFallbackParser : public BaseParser {
public:
    LiteralFallbackParser(Parser& parser);
    std::shared_ptr<AST::ASTNode> handle() override;
};

} // namespace ParserSpace

} // namespace DemoLang

#endif // DEMOLANG_PARSER