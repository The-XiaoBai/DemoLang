/**
 * @file src/parser/parser.cpp
 * @brief Transform tokens into AST.
**/

#include "ast.hpp"
#include "parser.hpp"
#include "utils.hpp"


namespace DemoLang {

namespace ParserSpace {

Parser::Parser() : current_pos(0) {}

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
    Utils::Chain<ASTNode> chain;
    // Assignment operators (lowest precedence)
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"="}));
    // Logical operators
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"&", "|"}));
    // Equality operators
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"==", "!="}));
    // Relational operators
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"<", "<=", ">", ">="}));
    // Additive operators
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"+", "-"}));
    // Multiplicative operators
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"*", "/"}));
    // Unary operators (highest precedence)
    chain.addHandler(std::make_shared<UnaryParser>(*this, std::vector<std::string>{"!", "-"}));
    // Primary expressions
    chain.addHandler(std::make_shared<ListParser>(*this));
    chain.addHandler(std::make_shared<FunctionCallParser>(*this));
    chain.addHandler(std::make_shared<IdentifierParser>(*this));
    chain.addHandler(std::make_shared<IfParser>(*this));
    chain.addHandler(std::make_shared<WhileParser>(*this));
    chain.addHandler(std::make_shared<LoopControlParser>(*this));
    chain.addHandler(std::make_shared<ParenthesizedParser>(*this));
    chain.addHandler(std::make_shared<LiteralFallbackParser>(*this));
    return chain.execute();
}

} // namespace DemoLang