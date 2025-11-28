/**
 * @file src/interpreter/interpreter.cpp
 * @brief Interpreter implementation.
 * @author The-XiaoBai
 * @date 2025/11/14
**/

#include "interpreter.hpp"


namespace DemoLang {

bool InterpreterSpace::Environment::has(const std::string& name) const {
    return scope.find(name) != scope.end();
}


std::shared_ptr<BaseType> InterpreterSpace::Environment::get(const std::string& name) const {
    auto it = scope.find(name);
    if (it != scope.end()) return it->second;
    return nullptr;
}


void InterpreterSpace::Environment::set(const std::string& name, const BaseType& value) {
    scope[name] = value.clone();
}


std::string InterpreterSpace::Interpreter::interpret(const std::shared_ptr<AST::ASTNode>& node) {
    return "";
}


void InterpreterSpace::Interpreter::visit(UnaryOpNode& node) {}
void InterpreterSpace::Interpreter::visit(BinaryOpNode& node) {}
void InterpreterSpace::Interpreter::visit(IdNode& node) {}
void InterpreterSpace::Interpreter::visit(IntNode& node) {}
void InterpreterSpace::Interpreter::visit(FloatNode& node) {}
void InterpreterSpace::Interpreter::visit(StringNode& node) {}
void InterpreterSpace::Interpreter::visit(ErrorNode& node) {}

} // namespace DemoLang
