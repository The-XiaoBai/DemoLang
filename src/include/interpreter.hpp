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

using namespace DemoLang::ValueTypes;
using namespace DemoLang::AST;
using Utils::Singleton;


/**
 * @brief Environment to store variables in the context of execution
**/
class Environment {
private:
    std::unordered_map<std::string, std::shared_ptr<BaseType>> scope;
    std::unordered_map<std::string, std::shared_ptr<ASTNode>> functions;

public:
    Environment();
    
    bool has(const std::string& name) const;
    std::shared_ptr<BaseType> get(const std::string& name) const;
    void set(const std::string& name, const BaseType& value);
    bool hasFunction(const std::string& name) const;
    std::shared_ptr<ASTNode> getFunction(const std::string& name) const;
    void setFunction(const std::string& name, std::shared_ptr<ASTNode> func);
    
    // Allow Interpreter to access scope directly
    friend class Interpreter;
};


/**
 * @brief Interpreter class
**/
class Interpreter : public Singleton<Interpreter>, public ASTVisitor {
    friend class Singleton<Interpreter>;

private:
    Environment env = Environment();
    std::shared_ptr<BaseType> result;

public:
    Interpreter();
    std::string interpret(const std::shared_ptr<ASTNode>& node);
    
    void visit(UnaryOpNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(IdNode& node) override;
    void visit(IntNode& node) override;
    void visit(FloatNode& node) override;
    void visit(StringNode& node) override;
    void visit(ErrorNode& node) override;
    void visit(FunctionCallNode& node) override;
    void visit(FunctionDefNode& node) override;
    void visit(IfNode& node) override;
    void visit(WhileNode& node) override;
    void visit(LoopControlNode& node) override;
    void visit(StatementSequenceNode& node) override;
    void visit(ListNode& node) override;
    void visit(IndexNode& node) override;
};

} // namespace InterpreterSpace

} // namespace DemoLang

#endif // DEMOLANG_INTERPRETER