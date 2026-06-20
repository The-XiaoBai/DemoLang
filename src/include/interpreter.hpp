/**
 * @file src/include/interpreter.hpp
 * @brief Basic declaration for interpreter.
**/

#pragma once
#ifndef DEMOLANG_INTERPRETER
#define DEMOLANG_INTERPRETER

#include "ast.hpp"
#include "builtins.hpp"
#include "utils.hpp"
#include <unordered_map>
#include <functional>


namespace DemoLang {

namespace InterpreterSpace {


/**
 * @brief Environment to store variables in the current execution context
**/
class Environment {
private:
    std::unordered_map<std::string, std::shared_ptr<ValueTypes::BaseType>> scope;
    std::unordered_map<std::string, std::shared_ptr<AST::ASTNode>> functions;

public:
    Environment();
    
    bool has(const std::string& name) const;
    std::shared_ptr<ValueTypes::BaseType> get(const std::string& name) const;
    void set(const std::string& name, const ValueTypes::BaseType& value);
    bool hasFunction(const std::string& name) const;
    std::shared_ptr<AST::ASTNode> getFunction(const std::string& name) const;
    void setFunction(const std::string& name, std::shared_ptr<AST::ASTNode> func);
    
    // Allow Interpreter to access scope directly
    friend class Interpreter;
};


/**
 * @brief Interpreter class
**/
class Interpreter : public Utils::Singleton<Interpreter>, public AST::ASTVisitor {
    friend class Utils::Singleton<Interpreter>;

private:
    Environment env = Environment();
    std::shared_ptr<ValueTypes::BaseType> result;

public:
    Interpreter();
    std::string interpret(const std::shared_ptr<AST::ASTNode>& node);
    
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