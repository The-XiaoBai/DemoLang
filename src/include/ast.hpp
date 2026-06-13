/**
 * @file src/include/ast.hpp
 * @brief Abstract Syntax Tree nodes and visitor interface.
**/

#pragma once
#ifndef DEMOLANG_AST
#define DEMOLANG_AST

#include <memory>
#include <vector>
#include <string>
#include "utils.hpp"


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
    virtual void visit(class FunctionCallNode& node) = 0;
    virtual void visit(class FunctionDefNode& node) = 0;
    virtual void visit(class IfNode& node) = 0;
    virtual void visit(class WhileNode& node) = 0;
    virtual void visit(class LoopControlNode& node) = 0;
    virtual void visit(class StatementSequenceNode& node) = 0;
    virtual void visit(class ListNode& node) = 0;
    virtual void visit(class IndexNode& node) = 0;
};


/**
 * @brief Base class for all AST nodes.
**/
struct ASTNode : public std::enable_shared_from_this<ASTNode> {
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
    UnaryOpNode(std::string op, std::shared_ptr<ASTNode> operand);
    void accept(ASTVisitor& visitor) override;
    const std::string& getOp() const;
    ASTNode* getOperand() const;
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
    BinaryOpNode(std::string op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right);
    void accept(ASTVisitor& visitor) override;
    const std::string& getOp() const;
    ASTNode* getLeft() const;
    ASTNode* getRight() const;
};


/**
 * @brief Node representing identifiers.
**/
class IdNode : public ASTNode {
private:
    std::string name;

public:
    explicit IdNode(const std::string& id);
    void accept(ASTVisitor& visitor) override;
    const std::string& getName() const;
};


/**
 * @brief Node representing integer literals.
**/
class IntNode : public ASTNode {
private:
    long long value;

public:
    explicit IntNode(long long val);
    void accept(ASTVisitor& visitor) override;
    long long getValue() const;
};


/**
 * @brief Node representing floating-point literals.
**/
class FloatNode : public ASTNode {
private:
    long double value;

public:
    explicit FloatNode(long double val);
    void accept(ASTVisitor& visitor) override;
    long double getValue() const;
};


/**
 * @brief Node representing string literals.
**/
class StringNode : public ASTNode {
private:
    std::string value;

public:
    explicit StringNode(const std::string& val);
    void accept(ASTVisitor& visitor) override;
    const std::string& getValue() const;
};


/**
 * @brief Node representing error conditions.
**/
class ErrorNode : public ASTNode {
private:
    std::string message;

public:
    explicit ErrorNode(const std::string& msg);
    void accept(ASTVisitor& visitor) override;
    const std::string& getMessage() const;
};


/**
 * @brief Node representing function calls.
**/
class FunctionCallNode : public ASTNode {
private:
    std::string name;
    std::shared_ptr<ASTNode> calleeNode;
    std::vector<std::shared_ptr<ASTNode>> args;

public:
    FunctionCallNode(const std::string& funcName, std::vector<std::shared_ptr<ASTNode>> arguments);
    FunctionCallNode(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> arguments);
    void accept(ASTVisitor& visitor) override;
    const std::string& getName() const;
    std::shared_ptr<ASTNode> getCalleeNode() const;
    const std::vector<std::shared_ptr<ASTNode>>& getArgs() const;
};

/**
 * @brief Node representing function definitions (including lambdas).
**/
class FunctionDefNode : public ASTNode {
private:
    std::string name;  // empty = anonymous lambda
    std::vector<std::string> params;
    std::vector<std::shared_ptr<ASTNode>> paramDefaults;
    std::shared_ptr<ASTNode> body;

public:
    FunctionDefNode(const std::string& funcName, std::vector<std::string> parameters, 
                   std::vector<std::shared_ptr<ASTNode>> defaults, std::shared_ptr<ASTNode> functionBody);
    void accept(ASTVisitor& visitor) override;
    const std::string& getName() const;
    const std::vector<std::string>& getParams() const;
    const std::vector<std::shared_ptr<ASTNode>>& getParamDefaults() const;
    ASTNode* getBody() const;
    bool isAnonymous() const;
};

/**
 * @brief Node representing if-else if-else statements.
 * Syntax: ?(condition){body} ??(condition){body} :{body}
**/
class IfNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> conditions;  // condition expressions
    std::vector<std::shared_ptr<ASTNode>> bodies;       // corresponding bodies
    std::shared_ptr<ASTNode> elseBody;                  // else branch (nullable)

public:
    IfNode(std::vector<std::shared_ptr<ASTNode>> conds,
           std::vector<std::shared_ptr<ASTNode>> bodys,
           std::shared_ptr<ASTNode> elseBod);
    void accept(ASTVisitor& visitor) override;
    const std::vector<std::shared_ptr<ASTNode>>& getConditions() const;
    const std::vector<std::shared_ptr<ASTNode>>& getBodies() const;
    ASTNode* getElseBody() const;
};

/**
 * @brief Node representing while loop statements.
 * Syntax: ^(condition){body}
**/
class WhileNode : public ASTNode {
private:
    std::shared_ptr<ASTNode> condition;
    std::shared_ptr<ASTNode> body;
    
public:
    WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> loopBody);
    void accept(ASTVisitor& visitor) override;
    ASTNode* getCondition() const;
    ASTNode* getBody() const;
};

/**
 * @brief Loop control type enum.
**/
enum class LoopControlType { Break, Continue };

/**
 * @brief Node representing break/continue statements.
 * Syntax: ## (break), # (continue)
**/
class LoopControlNode : public ASTNode {
private:
    LoopControlType type;
    
public:
    explicit LoopControlNode(LoopControlType t);
    void accept(ASTVisitor& visitor) override;
    bool isBreak() const;
    bool isContinue() const;
};

/**
 * @brief Node representing sequence of statements.
 * Syntax: statement1; statement2; ...
**/
class StatementSequenceNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> statements;
    
public:
    StatementSequenceNode(std::vector<std::shared_ptr<ASTNode>> stmts);
    void accept(ASTVisitor& visitor) override;
    const std::vector<std::shared_ptr<ASTNode>>& getStatements() const;
};


/**
 * @brief Node representing list literals.
 * Syntax: [1, 2, 3]
**/
class ListNode : public ASTNode {
private:
    std::vector<std::shared_ptr<ASTNode>> elements;
    
public:
    ListNode(std::vector<std::shared_ptr<ASTNode>> elems);
    void accept(ASTVisitor& visitor) override;
    const std::vector<std::shared_ptr<ASTNode>>& getElements() const;
};


/**
 * @brief Node representing index access.
 * Syntax: a[0]
**/
class IndexNode : public ASTNode {
private:
    std::shared_ptr<ASTNode> object;
    std::shared_ptr<ASTNode> index;
    
public:
    IndexNode(std::shared_ptr<ASTNode> obj, std::shared_ptr<ASTNode> idx);
    void accept(ASTVisitor& visitor) override;
    ASTNode* getObject() const;
    ASTNode* getIndex() const;
};

} // namespace AST

} // namespace DemoLang

#endif // DEMOLANG_AST