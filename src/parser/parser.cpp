/**
 * @file src/parser/parser.cpp
 * @brief Transform tokens into AST.
**/

#include "ast.hpp"
#include "parser.hpp"
#include "utils.hpp"


namespace DemoLang {

bool ParserSpace::Parser::match(TokenType type, const std::string& value) {
    // Check if current token matches expected type and value
    Token curr = current();
    bool matches = (curr.type == type && curr.value == value);
    if (matches) advance(); // Move to next token if match found
    return matches;
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
    // Create operator precedence chain using chain of responsibility pattern
    // Operators are added in order of precedence (lowest to highest)
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
    // Primary expressions (literals, identifiers)
    chain.addHandler(std::make_shared<PrimaryParser>(*this));
    return chain.execute();
}

} // namespace DemoLang