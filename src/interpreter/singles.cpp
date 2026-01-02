/**
 * @file src/interpreter/singles.cpp
 * @brief Visitor implementation for other single nodes.
 * @author The-XiaoBai
 * @date 2025/12/12
**/

#include "interpreter.hpp"


namespace DemoLang {

void InterpreterSpace::Interpreter::visit(IdNode& node) {
    result = env.has(node.getName()) ? env.get(node.getName())
        : std::make_shared<Exception>("Undefined variable: " + node.getName());
}

void InterpreterSpace::Interpreter::visit(IntNode& node) {
    result = std::make_shared<Integer>(node.getValue());
}

void InterpreterSpace::Interpreter::visit(FloatNode& node) {
    result = std::make_shared<Float>(node.getValue());
}

void InterpreterSpace::Interpreter::visit(StringNode& node) {
    result = std::make_shared<String>(node.getValue());
}

void InterpreterSpace::Interpreter::visit(ErrorNode& node) {
    result = std::make_shared<Exception>(node.getMsg());
}

} // namespace DemoLang