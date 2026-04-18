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
    } else if (auto list = dynamic_cast<List*>(result.get())) {
        auto items = std::any_cast<std::vector<std::shared_ptr<BaseType>>>(list->getValue());
        std::string res = "[";
        for (size_t i = 0; i < items.size(); ++i) {
            if (auto s = dynamic_cast<String*>(items[i].get())) {
                res += std::any_cast<std::string>(s->getValue());
            } else if (auto n = dynamic_cast<Integer*>(items[i].get())) {
                res += std::to_string(std::any_cast<long long>(n->getValue()));
            } else if (auto f = dynamic_cast<Float*>(items[i].get())) {
                res += std::to_string(std::any_cast<long double>(f->getValue()));
            } else if (auto l = dynamic_cast<List*>(items[i].get())) {
                // Recursive: delegate to list formatting
                std::string inner = "[";
                auto innerItems = std::any_cast<std::vector<std::shared_ptr<BaseType>>>(l->getValue());
                for (size_t j = 0; j < innerItems.size(); ++j) {
                    if (auto s = dynamic_cast<String*>(innerItems[j].get())) {
                        inner += std::any_cast<std::string>(s->getValue());
                    } else if (auto n = dynamic_cast<Integer*>(innerItems[j].get())) {
                        inner += std::to_string(std::any_cast<long long>(n->getValue()));
                    } else if (auto f = dynamic_cast<Float*>(innerItems[j].get())) {
                        inner += std::to_string(std::any_cast<long double>(f->getValue()));
                    } else if (auto il = dynamic_cast<List*>(innerItems[j].get())) {
                        inner += "[...]";
                    } else {
                        inner += "?";
                    }
                    if (j < innerItems.size() - 1) inner += ", ";
                }
                inner += "]";
                res += inner;
            } else {
                res += "?";
            }
            if (i < items.size() - 1) res += ", ";
        }
        res += "]";
        return res;
    } else {
        return "Unknown type";
    }
}

} // namespace DemoLang
