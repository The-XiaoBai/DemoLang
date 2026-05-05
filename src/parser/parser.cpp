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

} // namespace ParserSpace

Token ParserSpace::Parser::current() const { return current_pos < tokens.size() ? tokens[current_pos] : Token(TokenType::END, ""); }
void ParserSpace::Parser::advance() { if (current_pos < tokens.size()) current_pos++; }

bool ParserSpace::Parser::match(TokenType type, const std::string& value) {
    // Check if current token matches expected type and value
    Token curr = current();
    bool matches = (curr.type == type && curr.value == value);
    if (matches) advance(); // Move to next token if match found
    return matches;
}

size_t ParserSpace::Parser::savePosition() const {
    return current_pos;
}

void ParserSpace::Parser::restorePosition(size_t pos) {
    current_pos = pos;
}


std::shared_ptr<ASTNode> ParserSpace::Parser::parse(const std::vector<Token> &tokens) {
    // Initialize parser state
    this->tokens = tokens;
    this->current_pos = 0;
    
    try {
        // Start parsing from expression level
        return parseExpression();
    } catch (const std::exception& e) {
        // Return error node if parsing fails
        return std::make_shared<ErrorNode>(e.what());
    }
}

std::shared_ptr<ASTNode> ParserSpace::Parser::parseExpression() {
    // Parse sequence of statements separated by ';'
    std::vector<std::shared_ptr<ASTNode>> statements;
    
    // Parse first expression
    auto firstExpr = parseExpressionInternal();
    statements.push_back(firstExpr);
    
    // Parse additional statements separated by ';'
    while (current().type == TokenType::OPERATOR && current().value == ";") {
        advance(); // Consume ';'
        if (current().type == TokenType::END) {
            // Trailing semicolon at end
            break;
        }
        auto nextExpr = parseExpressionInternal();
        statements.push_back(nextExpr);
    }
    
    // Return single statement or sequence node
    if (statements.size() == 1) {
        return statements[0];
    } else {
        return std::make_shared<StatementSequenceNode>(statements);
    }
}

std::shared_ptr<ASTNode> ParserSpace::Parser::parseExpressionInternal() {
    return exprChain.execute();
}

std::tuple<std::string, std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
ParserSpace::Parser::parseOneParam() {
    if (current().type != TokenType::IDENTIFIER)
        return {"", nullptr, std::make_shared<ErrorNode>("Expected parameter name")};
    std::string paramName = current().value;
    advance();
    std::shared_ptr<ASTNode> defaultValue = nullptr;
    if (current().type == TokenType::OPERATOR && current().value == "=") {
        advance();
        defaultValue = parseExpressionInternal();
    }
    return {paramName, defaultValue, nullptr};
}

std::pair<std::shared_ptr<ASTNode>, std::shared_ptr<ASTNode>>
ParserSpace::Parser::parseOneArg() {
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

} // namespace DemoLang