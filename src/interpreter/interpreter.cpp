/**
 * @file src/interpreter/interpreter.cpp
 * @brief Interpreter implementation.
 * @author The-XiaoBai
 * @date 2025/12/12
**/

#include "interpreter.hpp"


namespace DemoLang {

bool InterpreterSpace::Environment::has(const std::string& name) const {
    return scope.find(name) != scope.end();
}


std::shared_ptr<BaseType> InterpreterSpace::Environment::get(const std::string& name) const {
    auto it = scope.find(name);
    if (it != scope.end()) return it->second;
    return std::make_shared<Exception>("Cannot find variable: " + name);
}


void InterpreterSpace::Environment::set(const std::string& name, const BaseType& value) {
    scope[name] = value.clone();
}


std::string InterpreterSpace::Interpreter::interpret(const std::shared_ptr<AST::ASTNode>& node) {
    if (!node) {
        auto result = std::make_shared<Exception>("Null AST Node");
        return std::any_cast<std::string>(result->getValue());
    }

    node->accept(*this);

    if (!result) {
        result = std::make_shared<Exception>("Failed to interpret");
    }
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
