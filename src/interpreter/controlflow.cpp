/**
 * @file src/interpreter/controlflow.cpp
 * @brief Visitor implementation for control flow nodes.
**/

#include "interpreter.hpp"

namespace DemoLang {

static bool isTruthy(const std::shared_ptr<BaseType>& val) {
    if (auto integer = dynamic_cast<Integer*>(val.get())) {
        return std::any_cast<long long>(integer->getValue()) != 0;
    } else if (auto flo = dynamic_cast<Float*>(val.get())) {
        return std::any_cast<long double>(flo->getValue()) != 0.0;
    } else if (auto str = dynamic_cast<String*>(val.get())) {
        return !std::any_cast<std::string>(str->getValue()).empty();
    }
    return false;
}

void InterpreterSpace::Interpreter::visit(IfNode& node) {
    for (size_t i = 0; i < node.getConditions().size(); ++i) {
        node.getConditions()[i]->accept(*this);
        if (isTruthy(result)) {
            node.getBodies()[i]->accept(*this);
            return;
        }
    }
    if (node.getElseBody()) {
        node.getElseBody()->accept(*this);
    } else {
        result = std::make_shared<String>("");
    }
}

void InterpreterSpace::Interpreter::visit(WhileNode& node) {
    while (true) {
        node.getCondition()->accept(*this);
        if (!isTruthy(result)) {
            result = std::make_shared<String>("");
            return;
        }

        node.getBody()->accept(*this);

        if (auto exc = dynamic_cast<Exception*>(result.get())) {
            auto msg = std::any_cast<std::string>(exc->getValue());
            if (msg == "__break__") {
                result = std::make_shared<String>("");
                return;
            } else if (msg == "__continue__") {
                result = std::make_shared<String>("");
                continue;
            }
            return;
        }
    }
}

void InterpreterSpace::Interpreter::visit(LoopControlNode& node) {
    result = std::make_shared<Exception>(node.isBreak() ? "__break__" : "__continue__");
}

void InterpreterSpace::Interpreter::visit(StatementSequenceNode& node) {
    std::shared_ptr<BaseType> lastResult = std::make_shared<String>("");

    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
        lastResult = result;

        if (auto exc = dynamic_cast<Exception*>(lastResult.get())) {
            auto msg = std::any_cast<std::string>(exc->getValue());
            if (msg == "__break__" || msg == "__continue__") {
                result = lastResult;
                return;
            }
        }
    }

    result = lastResult;
}

} // namespace DemoLang
