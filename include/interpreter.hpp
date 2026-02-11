/**
 * @file include/interpreter.hpp
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

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::AST;
using namespace DemoLang::ValueTypes;


namespace DemoLang {

namespace InterpreterSpace {

/**
 * @brief Environment to store variables in the context of execution
**/
class Environment {
private:
    std::unordered_map<std::string, std::shared_ptr<BaseType>> scope;

public:
    Environment() = default;
    
    bool has(const std::string& name) const;
    std::shared_ptr<BaseType> get(const std::string& name) const;
    void set(const std::string& name, const BaseType& value);
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
    Interpreter() = default;
    std::string interpret(const std::shared_ptr<ASTNode>& node);
    
    void visit(UnaryOpNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(IdNode& node) override;
    void visit(IntNode& node) override;
    void visit(FloatNode& node) override;
    void visit(StringNode& node) override;
    void visit(ErrorNode& node) override;
};

} // namespace InterpreterSpace

} // namespace DemoLang

#endif // DEMOLANG_INTERPRETER