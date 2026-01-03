/**
 * @file include/ast.hpp
 * @brief Abstract Syntax Tree nodes and visitor interface.
 * @author The-XiaoBai
 * @date 2025/11/16
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
    
    virtual void visit(class UnaryOpNode& node) = 0;
    virtual void visit(class BinaryOpNode& node) = 0;
    virtual void visit(class IdNode& node) = 0;
    virtual void visit(class IntNode& node) = 0;
    virtual void visit(class FloatNode& node) = 0;
    virtual void visit(class StringNode& node) = 0;
    virtual void visit(class ErrorNode& node) = 0;
};


/**
 * @brief Base class for all AST nodes.
**/
struct ASTNode {
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};


/**
 * @brief Node representing unary operations.
**/
class UnaryOpNode : public ASTNode {
private:
    std::string op;
    std::shared_ptr<ASTNode> operand;
    
public:
    UnaryOpNode(std::string op, std::shared_ptr<ASTNode> operand)
        : op(std::move(op)), operand(std::move(operand)) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }

    const std::string& getOp() const { return op; }
    ASTNode* getOperand() const { return operand.get(); }
};


/**
 * @brief Node representing binary operations.
**/
class BinaryOpNode : public ASTNode {
private:
    std::string op;
    std::shared_ptr<ASTNode> left;
    std::shared_ptr<ASTNode> right;
    
public:
    BinaryOpNode(std::string op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
        : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    const std::string& getOp() const { return op; }
    ASTNode* getLeft() const { return left.get(); }
    ASTNode* getRight() const { return right.get(); }
};


/**
 * @brief Node representing identifiers.
**/
class IdNode : public ASTNode {
private:
    std::string name;

public:
    IdNode(const std::string& id) : name(id) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    const std::string& getName() const { return name; }
};


/**
 * @brief Node representing integer literals.
**/
class IntNode : public ASTNode {
private:
    long long value;

public:
    explicit IntNode(long long val) : value(val) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    long long getValue() const { return value; }
};


/**
 * @brief Node representing floating-point literals.
**/
class FloatNode : public ASTNode {
private:
    long double value;

public:
    explicit FloatNode(long double val) : value(val) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    long double getValue() const { return value; }
};


/**
 * @brief Node representing string literals.
**/
class StringNode : public ASTNode {
private:
    std::string value;

public:
    explicit StringNode(const std::string& val) : value(val) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    const std::string& getValue() const { return value; }
};


/**
 * @brief Node representing error conditions.
**/
class ErrorNode : public ASTNode {
private:
    std::string message;

public:
    explicit ErrorNode(const std::string& msg) : message(msg) {}
    void accept(ASTVisitor& visitor) override { visitor.visit(*this); }
    const std::string& getMessage() const { return message; }
};

} // namespace AST

} // namespace DemoLang

#endif // DEMOLANG_AST