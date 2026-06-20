/**
 * @file src/parser/parser.cpp
 * @brief Transform tokens into AST.
**/

#include "ast.hpp"
#include "parser.hpp"
#include "utils.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::Tokens;
using namespace DemoLang::AST;

namespace DemoLang {
namespace ParserSpace {

Parser::Parser() : current_pos(0) {
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"="}));
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"&", "|"}));
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"==", "!="}));
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"<", "<=", ">", ">="}));
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"+", "-"}));
    exprChain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"*", "/"}));
    exprChain.addHandler(std::make_shared<UnaryParser>(*this, std::vector<std::string>{"!", "-"}));
    exprChain.addHandler(std::make_shared<ListParser>(*this));
    exprChain.addHandler(std::make_shared<FunctionCallParser>(*this));
    exprChain.addHandler(std::make_shared<IdentifierParser>(*this));
    exprChain.addHandler(std::make_shared<IfParser>(*this));
    exprChain.addHandler(std::make_shared<WhileParser>(*this));
    exprChain.addHandler(std::make_shared<LoopControlParser>(*this));
    exprChain.addHandler(std::make_shared<ParenthesizedParser>(*this));
    exprChain.addHandler(std::make_shared<LiteralFallbackParser>(*this));
}

BaseParser::BaseParser(Parser& parser) : parser(parser) {}

UnaryParser::UnaryParser(Parser& parser, std::vector<std::string> operators)
    : BaseParser(parser), operators(operators) {}

BinaryParser::BinaryParser(Parser& parser, std::vector<std::string> operators)
    : BaseParser(parser), operators(operators) {}

Token Parser::current() const { return current_pos < tokens.size() ? tokens[current_pos] : Token(TokenType::END, ""); }
void Parser::advance() { if (current_pos < tokens.size()) current_pos++; }

bool Parser::match(TokenType type, const std::string& value) {
    Token curr = current();
    bool matches = (curr.type == type && curr.value == value);
    if (matches) advance();
    return matches;
}

size_t Parser::savePosition() const {
    return current_pos;
}

void Parser::restorePosition(size_t pos) {
    current_pos = pos;
}

std::shared_ptr<ASTNode> Parser::parse(const std::vector<Token> &tokens) {
    this->tokens = tokens;
    this->current_pos = 0;

    try {
        return parseExpression();
    } catch (const std::exception& e) {
        return std::make_shared<ErrorNode>(e.what());
    }
}

std::shared_ptr<ASTNode> Parser::parseExpression() {
    std::vector<std::shared_ptr<ASTNode>> statements;

    // Parse first expression
    auto firstExpr = exprChain.execute();
    statements.push_back(firstExpr);

    // Parse additional statements separated by ';'
    while (current().type == TokenType::OPERATOR && current().value == ";") {
        advance();  // Consume ';'
        if (current().type == TokenType::END) break;  // Trailing semicolon at end
        auto nextExpr = exprChain.execute();
        statements.push_back(nextExpr);
    }

    if (statements.size() == 1) {
        return statements[0];
    } else {
        return std::make_shared<StatementSequenceNode>(statements);
    }
}

std::tuple<std::string, std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
Parser::parseOneParam() {
    if (current().type != TokenType::IDENTIFIER)
        return {"", nullptr, std::make_shared<ErrorNode>("Expected parameter name")};
    std::string paramName = current().value;
    advance();
    std::shared_ptr<ASTNode> defaultValue = nullptr;
    if (current().type == TokenType::OPERATOR && current().value == "=") {
        advance();
        defaultValue = exprChain.execute();
    }
    return {paramName, defaultValue, nullptr};
}

std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
Parser::parseOneArg() {
    if (current().type == TokenType::IDENTIFIER) {
        std::string idName = current().value;
        advance();
        if (current().type == TokenType::OPERATOR && current().value == "[") {
            advance();
            auto indexExpr = parseExpression();
            if (current().type != TokenType::OPERATOR || current().value != "]")
                return {nullptr, std::make_shared<ErrorNode>("Expected ']' in index access")};
            advance();
            return {std::make_shared<IndexNode>(std::make_shared<IdNode>(idName), indexExpr), nullptr};
        }
        if (current().type == TokenType::OPERATOR && current().value == "=") {
            advance();
            return {parseExpression(), nullptr};
        }
        return {std::make_shared<IdNode>(idName), nullptr};
    }
    return {parseExpression(), nullptr};
}

} // namespace ParserSpace
} // namespace DemoLang
