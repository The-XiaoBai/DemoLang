/**
 * @file src/interpreter/interpreter.cpp
 * @brief Interpreter implementation.
**/

#include "interpreter.hpp"


namespace DemoLang {

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


std::string InterpreterSpace::Interpreter::interpret(const std::shared_ptr<AST::ASTNode>& node) {
    // Handle null AST node
    if (!node) {
        auto result = std::make_shared<Exception>("Null AST Node");
        return std::any_cast<std::string>(result->getValue());
    }

    // Start AST traversal using visitor pattern
    node->accept(*this);

    // Handle interpretation result
    if (!result) {
        result = std::make_shared<Exception>("Failed to interpret");
    }
    
    // Convert result to string representation based on type
    if (auto exc = dynamic_cast<Exception*>(result.get())) {
        return std::any_cast<std::string>(exc->getValue());
    } else if (auto str = dynamic_cast<String*>(result.get())) {
        return std::any_cast<std::string>(str->getValue());
    } else if (auto integer = dynamic_cast<Integer*>(result.get())) {
        return std::to_string(std::any_cast<long long>(integer->getValue()));
    } else if (auto flo = dynamic_cast<Float*>(result.get())) {
        return std::to_string(std::any_cast<long double>(flo->getValue()));
    } else {
        return "Unknown type";
    }
}

} // namespace DemoLang
