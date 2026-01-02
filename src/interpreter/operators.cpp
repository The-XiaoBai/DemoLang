/**
 * @file src/interpreter/operators.cpp
 * @brief Visitor implementation for operators.
 * @author The-XiaoBai
 * @date 2025/12/12
**/

#include "interpreter.hpp"


namespace DemoLang {

void InterpreterSpace::Interpreter::visit(UnaryOpNode& node) {
    node.getOperand()->accept(*this);
    std::shared_ptr<BaseType> operand = result;

    if (operand->getName() != "Integer" && operand->getName() != "Float") {
        result = std::make_shared<Exception>("Operand must be numeric");
        return;
    }

    if (node.getOp() == "-") {
        if (operand->getName() == "Integer") {
            result = std::make_shared<Integer>(-std::any_cast<long long>(operand->getValue()));
        } else if (operand->getName() == "Float") {
            result = std::make_shared<Float>(-std::any_cast<long double>(operand->getValue()));
        }
    } else if (node.getOp() == "!") {
        if (operand->getName() == "Integer") {
            result = std::make_shared<Integer>(std::any_cast<long long>(operand->getValue()) == 0 ? 1 : 0);
        } else if (operand->getName() == "Float") {
            result = std::make_shared<Integer>(std::any_cast<long double>(operand->getValue()) == 0.0 ? 1 : 0);
        }
    } else {
        result = std::make_shared<Exception>("Unsupported operator");
    }
}


void InterpreterSpace::Interpreter::visit(BinaryOpNode& node) {
    node.getLeft()->accept(*this);
    std::shared_ptr<BaseType> left = result;
    node.getRight()->accept(*this);
    std::shared_ptr<BaseType> right = result;
    
    // Handle assignment separately (special case)
    if (node.getOp() == "=") {
        if (auto* identifier = dynamic_cast<IdNode*>(node.getLeft())) {
            std::string name = identifier->getName();
            env.set(name, *right);
            result = right;
        } else {
            result = std::make_shared<Exception>("Left side of assignment must be an identifier");
        }
        return;
    }
    
    // Use factory for other operators
    result = BinOperatorFactory::execute(node.getOp(), left, right);
}

} // namespace DemoLang