/**
 * @file src/include/interpreter.hpp
 * @brief Basic declaration for interpreter.
**/

#pragma once
#ifndef DEMOLANG_INTERPRETER
#define DEMOLANG_INTERPRETER

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>
#include "ast.hpp"
#include "builtins.hpp"
#include "utils.hpp"

namespace DemoLang {

namespace InterpreterSpace {

/**
 * @brief Environment to store variables in the current execution context.
 *        Supports a parent-child chain for lexical scoping.
**/
class Environment {
private:
    Environment* parent;
    std::unordered_map<std::string, std::shared_ptr<ValueTypes::BaseType>> scope;
    std::unordered_map<std::string, std::shared_ptr<AST::ASTNode>> functions;

public:
    Environment();
    explicit Environment(Environment* p);
    
    // Walk the parent chain to find a variable
    bool has(const std::string& name) const;
    // Check only the current scope (no parent walk)
    bool hasInCurrentScope(const std::string& name) const;
    std::shared_ptr<ValueTypes::BaseType> get(const std::string& name) const;
    // Set: write to the scope where the variable was originally defined;
    // if not found anywhere, set in the current scope
    void set(const std::string& name, const ValueTypes::BaseType& value);
    // Set strictly in the current scope (used for parameter binding to shadow outer vars)
    void setInCurrentScope(const std::string& name, const ValueTypes::BaseType& value);
    
    // Function management (walks parent chain for lookup, sets in current)
    bool hasFunction(const std::string& name) const;
    std::shared_ptr<AST::ASTNode> getFunction(const std::string& name) const;
    void setFunction(const std::string& name, std::shared_ptr<AST::ASTNode> func);
    
    // Clear all variables and functions (for test isolation)
    void clear();
};

/**
 * @brief Interpreter class
**/
class Interpreter : public Utils::Singleton<Interpreter>, public AST::ASTVisitor {
    friend class Utils::Singleton<Interpreter>;

private:
    // Root environment holds global variables and function definitions
    Environment rootEnv;
    // Pointer to the current environment (root or a child during function calls)
    Environment* env;
    // Stack of child environments owned by the interpreter
    std::vector<std::unique_ptr<Environment>> envStack;
    std::shared_ptr<ValueTypes::BaseType> result;

    // Push a new child environment onto the stack (for function call scoping)
    void pushEnv();
    // Pop the current child environment (restore parent)
    void popEnv();

public:
    Interpreter();
    std::string interpret(const std::shared_ptr<AST::ASTNode>& node);
    
    // Clear the environment (for test isolation)
    void clearEnvironment();
    
    void visit(AST::UnaryOpNode& node) override;
    void visit(AST::BinaryOpNode& node) override;
    void visit(AST::IdNode& node) override;
    void visit(AST::IntNode& node) override;
    void visit(AST::FloatNode& node) override;
    void visit(AST::StringNode& node) override;
    void visit(AST::ErrorNode& node) override;
    void visit(AST::FunctionCallNode& node) override;
    void visit(AST::FunctionDefNode& node) override;
    void visit(AST::IfNode& node) override;
    void visit(AST::WhileNode& node) override;
    void visit(AST::LoopControlNode& node) override;
    void visit(AST::StatementSequenceNode& node) override;
    void visit(AST::ListNode& node) override;
    void visit(AST::IndexNode& node) override;
};

} // namespace InterpreterSpace

} // namespace DemoLang

#endif // DEMOLANG_INTERPRETER