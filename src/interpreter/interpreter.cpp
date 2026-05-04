/**
 * @file src/interpreter/interpreter.cpp
 * @brief Interpreter implementation.
**/

#include "interpreter.hpp"


namespace DemoLang {

namespace InterpreterSpace {

Environment::Environment() = default;

Interpreter::Interpreter() = default;

} // namespace InterpreterSpace

bool InterpreterSpace::Environment::has(const std::string& name) const {
    // Check if variable exists in current scope
    return scope.find(name) != scope.end();
}


std::shared_ptr<BaseType> InterpreterSpace::Environment::get(const std::string& name) const {
    // Retrieve variable value from scope
    auto it = scope.find(name);
    if (it != scope.end()) return it->second;
    // Return exception if variable not found
    return std::make_shared<Exception>("Cannot find variable: " + name);
}


void InterpreterSpace::Environment::set(const std::string& name, const BaseType& value) {
    // Store variable in scope with cloned value
    scope[name] = value.clone();
}

bool InterpreterSpace::Environment::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

std::shared_ptr<ASTNode> InterpreterSpace::Environment::getFunction(const std::string& name) const {
    auto it = functions.find(name);
    if (it != functions.end()) return it->second;
    return nullptr;
}

void InterpreterSpace::Environment::setFunction(const std::string& name, std::shared_ptr<ASTNode> func) {
    functions[name] = func;
}

std::string InterpreterSpace::Interpreter::interpret(const std::shared_ptr<AST::ASTNode>& node) {
    if (!node) {
        return "Null AST Node";
    }
    node->accept(*this);
    if (!result) {
        return "Failed to interpret";
    }
    return result->toString();
}

} // namespace DemoLang
