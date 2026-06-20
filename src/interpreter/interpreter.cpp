/**
 * @file src/interpreter/interpreter.cpp
 * @brief Interpreter implementation.
**/

#include "interpreter.hpp"

using namespace DemoLang::ValueTypes;
using namespace DemoLang::AST;

namespace DemoLang {
namespace InterpreterSpace {

// Environment manages variable scope and function definitions
Environment::Environment() = default;
Interpreter::Interpreter() = default;

// Variable management
bool Environment::has(const std::string& name) const {
    return scope.find(name) != scope.end();
}

std::shared_ptr<BaseType> Environment::get(const std::string& name) const {
    auto it = scope.find(name);
    if (it != scope.end()) return it->second;
    return std::make_shared<Exception>("Undefined variable: " + name);
}

void Environment::set(const std::string& name, const BaseType& value) {
    scope[name] = value.clone();  // Store a copy of the value
}

// Function management
bool Environment::hasFunction(const std::string& name) const {
    return functions.find(name) != functions.end();
}

std::shared_ptr<ASTNode> Environment::getFunction(const std::string& name) const {
    auto it = functions.find(name);
    if (it != functions.end()) return it->second;
    return nullptr;
}

void Environment::setFunction(const std::string& name, std::shared_ptr<ASTNode> func) {
    functions[name] = func;
}

// Main interpretation entry point
std::string Interpreter::interpret(const std::shared_ptr<AST::ASTNode>& node) {
    if (!node) {
        return "Null AST Node";
    }
    node->accept(*this);  // Start AST traversal using Visitor pattern
    if (!result) {
        return "Failed to interpret";
    }
    return result->toString();
}

} // namespace InterpreterSpace
} // namespace DemoLang
