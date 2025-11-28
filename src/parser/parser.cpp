/**
 * @file src/parser/parser.cpp
 * @brief Transform tokens into an Abstract Syntax Tree (AST).
 * @author The-XiaoBai
 * @date 2025/11/16
**/

#include "ast.hpp"
#include "parser.hpp"
#include "utils.hpp"


namespace DemoLang {

bool ParserSpace::Parser::match(TokenType type, const std::string& value) {
    Token curr = current();
    bool matches = (curr.type == type && curr.value == value);
    if (matches) advance();
    return matches;
}


std::shared_ptr<ASTNode> ParserSpace::Parser::parse(const std::vector<Token> &tokens) {
    this->tokens = tokens;
    this->current_pos = 0;
    
    try {
        return parseExpression();
    } catch (const std::exception& e) {
        return std::make_shared<ErrorNode>(e.what());
    }
}

std::shared_ptr<ASTNode> ParserSpace::Parser::parseExpression() {
    Utils::Chain<ASTNode> chain;
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"="}));
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"&", "|"}));
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"==", "!="}));
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"<", "<=", ">", ">="}));
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"+", "-"}));
    chain.addHandler(std::make_shared<BinaryParser>(*this, std::vector<std::string>{"*", "/"}));
    chain.addHandler(std::make_shared<UnaryParser>(*this, std::vector<std::string>{"!", "-"}));
    chain.addHandler(std::make_shared<PrimaryParser>(*this));
    return chain.execute();
}

} // namespace DemoLang