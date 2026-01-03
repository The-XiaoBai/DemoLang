/**
 * @file src/interpreter/operators.cpp
 * @brief Visitor implementation for operators.
 * @author The-XiaoBai
 * @date 2025/12/12
**/

#include "interpreter.hpp"


namespace DemoLang {

void InterpreterSpace::Interpreter::visit(UnaryOpNode& node) {
    // First evaluate the operand
    node.getOperand()->accept(*this);
    std::shared_ptr<BaseType> operand = result;

    // Type validation: unary operators only work on numeric types
    if (operand->getName() != "Integer" && operand->getName() != "Float") {
        result = std::make_shared<Exception>("Operand must be numeric");
        return;
    }

    // Handle different unary operators
    if (node.getOp() == "-") {
        // Unary minus: negate the numeric value
        if (operand->getName() == "Integer") {
            result = std::make_shared<Integer>(-std::any_cast<long long>(operand->getValue()));
        } else if (operand->getName() == "Float") {
            result = std::make_shared<Float>(-std::any_cast<long double>(operand->getValue()));
        }
    } else if (node.getOp() == "!") {
        // Logical NOT: convert to boolean (0 = false, non-zero = true), then invert
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
    // Evaluate left operand first
    node.getLeft()->accept(*this);
    std::shared_ptr<BaseType> left = result;
    // Then evaluate right operand
    node.getRight()->accept(*this);
    std::shared_ptr<BaseType> right = result;
    
    // Handle assignment operator separately (special case with side effects)
    if (node.getOp() == "=") {
        if (auto* identifier = dynamic_cast<IdNode*>(node.getLeft())) {
            std::string name = identifier->getName();
            env.set(name, *right); // Store value in environment
            result = right; // Assignment expression returns the assigned value
        } else {
            result = std::make_shared<Exception>("Left side of assignment must be an identifier");
        }
        return;
    }
    
    // For all other binary operators, use the factory pattern
    result = BinOperatorFactory::execute(node.getOp(), left, right);
}

} // namespace DemoLang