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
Environment::Environment() : parent(nullptr) {}
Environment::Environment(Environment* p) : parent(p) {}

Interpreter::Interpreter() : env(&rootEnv) {}

// Variable management — walk parent chain
bool Environment::has(const std::string& name) const {
    if (scope.find(name) != scope.end()) return true;
    return parent ? parent->has(name) : false;
}

bool Environment::hasInCurrentScope(const std::string& name) const {
    return scope.find(name) != scope.end();
}

std::shared_ptr<BaseType> Environment::get(const std::string& name) const {
    auto it = scope.find(name);
    if (it != scope.end()) return it->second;
    if (parent) return parent->get(name);
    return std::make_shared<Exception>("Undefined variable: " + name);
}

// set: find the scope where the variable was originally defined and write there;
// if not found anywhere, write in the current scope
void Environment::set(const std::string& name, const BaseType& value) {
    if (hasInCurrentScope(name)) {
        scope[name] = value.clone();
        return;
    }
    if (parent && parent->has(name)) {
        parent->set(name, value);
        return;
    }
    scope[name] = value.clone();
}

// setInCurrentScope: always write in the current scope (used for parameter binding
// so that parameter names shadow outer variables)
void Environment::setInCurrentScope(const std::string& name, const BaseType& value) {
    scope[name] = value.clone();
}

// Clear all variables and functions
void Environment::clear() {
    scope.clear();
    functions.clear();
}

// Function management — walk parent chain for lookup, set in current
bool Environment::hasFunction(const std::string& name) const {
    if (functions.find(name) != functions.end()) return true;
    return parent ? parent->hasFunction(name) : false;
}

std::shared_ptr<ASTNode> Environment::getFunction(const std::string& name) const {
    auto it = functions.find(name);
    if (it != functions.end()) return it->second;
    if (parent) return parent->getFunction(name);
    return nullptr;
}

void Environment::setFunction(const std::string& name, std::shared_ptr<ASTNode> func) {
    functions[name] = func;
}

// Push/pop child environment stack
void Interpreter::pushEnv() {
    auto child = std::make_unique<Environment>(env);
    envStack.push_back(std::move(child));
    env = envStack.back().get();
}

void Interpreter::popEnv() {
    envStack.pop_back();
    env = envStack.empty() ? &rootEnv : envStack.back().get();
}

// Clear the environment (for test isolation)
void Interpreter::clearEnvironment() {
    rootEnv.clear();
    envStack.clear();
    env = &rootEnv;
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
