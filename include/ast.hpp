/**
 * @file include/ast.hpp
 * @brief Abstract Syntax Tree nodes and visitor interface.
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#pragma once
#ifndef DEMOLANG_AST
#define DEMOLANG_AST

#include <memory>
#include <vector>
#include <string>


namespace DemoLang {

namespace AST {

/**
 * @brief Visitor interface for AST traversal.
**/
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(class ANode& node) = 0;
};


/**
 * @brief Base class for all AST nodes.
**/
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};


class ANode : public ASTNode {
public:
    ANode() {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
};

} // namespace AST

} // namespace DemoLang

#endif // DEMOLANG_AST