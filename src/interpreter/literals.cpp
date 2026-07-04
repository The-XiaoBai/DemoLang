/**
 * @file src/interpreter/literals.cpp
 * @brief Visitor implementation for literal nodes.
**/

#include "interpreter.hpp"

using namespace DemoLang::ValueTypes;
using namespace DemoLang::AST;

namespace DemoLang {
namespace InterpreterSpace {

void Interpreter::visit(IntNode& node) {
    result = std::make_shared<Integer>(node.getValue());
}

void Interpreter::visit(FloatNode& node) {
    result = std::make_shared<Float>(node.getValue());
}

void Interpreter::visit(StringNode& node) {
    result = std::make_shared<String>(node.getValue());
}

void Interpreter::visit(ErrorNode& node) {
    result = std::make_shared<Exception>(node.getMessage());
}

void Interpreter::visit(ListNode& node) {
    std::vector<std::shared_ptr<BaseType>> elements;
    for (const auto& elem : node.getElements()) {
        elem->accept(*this);
        if (dynamic_cast<Exception*>(result.get())) return;  // Propagate exception from element evaluation
        elements.push_back(result);
    }
    result = std::make_shared<List>(elements);
}

void Interpreter::visit(IndexNode& node) {
    node.getObject()->accept(*this);
    auto obj = result;
    if (dynamic_cast<Exception*>(obj.get())) return;  // Propagate exception from object evaluation

    node.getIndex()->accept(*this);
    auto idx = result;
    if (dynamic_cast<Exception*>(idx.get())) return;  // Propagate exception from index evaluation

    auto list = dynamic_cast<List*>(obj.get());
    if (!list) {
        result = std::make_shared<Exception>("Cannot index non-list type");
        return;
    }

    auto integer = dynamic_cast<Integer*>(idx.get());
    if (!integer) {
        result = std::make_shared<Exception>("Index must be integer");
        return;
    }

    auto items = std::any_cast<std::vector<std::shared_ptr<BaseType>>>(list->getValue());
    long long pos = std::any_cast<long long>(integer->getValue());

    if (pos < 0 || static_cast<size_t>(pos) >= items.size()) {
        result = std::make_shared<Exception>("Index out of bounds");
        return;
    }

    result = items[static_cast<size_t>(pos)];
}

} // namespace InterpreterSpace
} // namespace DemoLang
