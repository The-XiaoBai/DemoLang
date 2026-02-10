/**
 * @file src/interpreter/operators.cpp
 * @brief Visitor implementation for operators.
**/

#include "interpreter.hpp"


namespace DemoLang {

// Helper functions for type checking and conversion
auto isNumeric = [](std::shared_ptr<BaseType> operand) -> bool {
    return operand->getName() == "Integer" || operand->getName() == "Float";
};

auto toInt = [](std::shared_ptr<BaseType> operand) -> std::shared_ptr<BaseType> {
    if (!operand) {
        return std::make_shared<Exception>("Null operand provided");
    }
    if (operand->getName() == "Integer") {
        return operand;
    }
    return std::make_shared<Exception>("Cannot convert non-integer to integer");
};

auto toFloat = [](std::shared_ptr<BaseType> operand) -> std::shared_ptr<BaseType> {
    if (!operand) {
        return std::make_shared<Exception>("Null operand provided");
    }
    if (operand->getName() == "Integer") {
        return std::make_shared<Float>(static_cast<long double>(std::any_cast<long long>(operand->getValue())));
    } else if (operand->getName() == "Float") {
        return operand;
    }
    return std::make_shared<Exception>("Cannot convert non-numeric to float");
};

// Arithmetic operators
class AddOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (left->getName() == "String" && right->getName() == "String") {
            try {
                std::string leftStr = std::any_cast<std::string>(left->getValue());
                std::string rightStr = std::any_cast<std::string>(right->getValue());
                return std::make_shared<String>(leftStr + rightStr);
            } catch (const std::bad_any_cast& e) {
                return std::make_shared<Exception>("String type conversion error: " + std::string(e.what()));
            }
        } 
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric or both strings");
        }
        if (left->getName() == "Float" || right->getName() == "Float") {
            auto leftVal = toFloat(left);
            auto rightVal = toFloat(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Float>(std::any_cast<long double>(leftVal->getValue()) + std::any_cast<long double>(rightVal->getValue()));
        } else {
            auto leftVal = toInt(left);
            auto rightVal = toInt(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Integer>(std::any_cast<long long>(leftVal->getValue()) + std::any_cast<long long>(rightVal->getValue()));
        }
    }
};

class SubOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        if (left->getName() == "Float" || right->getName() == "Float") {
            auto leftVal = toFloat(left);
            auto rightVal = toFloat(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Float>(std::any_cast<long double>(leftVal->getValue()) - std::any_cast<long double>(rightVal->getValue()));
        } else {
            auto leftVal = toInt(left);
            auto rightVal = toInt(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Integer>(std::any_cast<long long>(leftVal->getValue()) - std::any_cast<long long>(rightVal->getValue()));
        }
    }
};

class MulOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        if (left->getName() == "Float" || right->getName() == "Float") {
            auto leftVal = toFloat(left);
            auto rightVal = toFloat(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Float>(std::any_cast<long double>(leftVal->getValue()) * std::any_cast<long double>(rightVal->getValue()));
        } else {
            auto leftVal = toInt(left);
            auto rightVal = toInt(right);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            return std::make_shared<Integer>(std::any_cast<long long>(leftVal->getValue()) * std::any_cast<long long>(rightVal->getValue()));
        }
    }
};

class DivOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        auto rightVal = toFloat(right);
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        
        long double rightNum = std::any_cast<long double>(rightVal->getValue());
        if (rightNum == 0.0) {
            return std::make_shared<Exception>("Division by zero");
        }
        
        auto leftVal = toFloat(left);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        
        return std::make_shared<Float>(std::any_cast<long double>(leftVal->getValue()) / rightNum);
    }
};

// Comparison operators
class EqOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (left->getName() == "String" && right->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            return std::make_shared<Integer>(leftStr == rightStr ? 1 : 0);
        }
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be both numeric or both strings");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) == std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

class NeqOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (left->getName() == "String" && right->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            return std::make_shared<Integer>(leftStr != rightStr ? 1 : 0);
        }
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be both numeric or both strings");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) != std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

class GtOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) > std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

class LtOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) < std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

class GteOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) >= std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

class LteOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        auto leftVal = toFloat(left);
        auto rightVal = toFloat(right);
        if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
        if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
        return std::make_shared<Integer>(std::any_cast<long double>(leftVal->getValue()) <= std::any_cast<long double>(rightVal->getValue()) ? 1 : 0);
    }
};

// Logical operators
class AndOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        bool leftBool = false, rightBool = false;
        
        if (isNumeric(left)) {
            auto leftVal = toFloat(left);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            leftBool = std::any_cast<long double>(leftVal->getValue()) != 0.0;
        } else if (left->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            leftBool = !leftStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical AND");
        }
        
        if (isNumeric(right)) {
            auto rightVal = toFloat(right);
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            rightBool = std::any_cast<long double>(rightVal->getValue()) != 0.0;
        } else if (right->getName() == "String") {
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            rightBool = !rightStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical AND");
        }
        
        return std::make_shared<Integer>(leftBool && rightBool ? 1 : 0);
    }
};

class OrOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        bool leftBool = false, rightBool = false;
        
        if (isNumeric(left)) {
            auto leftVal = toFloat(left);
            if (auto leftExc = dynamic_cast<Exception*>(leftVal.get())) return leftVal;
            leftBool = std::any_cast<long double>(leftVal->getValue()) != 0.0;
        } else if (left->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            leftBool = !leftStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical OR");
        }
        
        if (isNumeric(right)) {
            auto rightVal = toFloat(right);
            if (auto rightExc = dynamic_cast<Exception*>(rightVal.get())) return rightVal;
            rightBool = std::any_cast<long double>(rightVal->getValue()) != 0.0;
        } else if (right->getName() == "String") {
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            rightBool = !rightStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical OR");
        }
        
        return std::make_shared<Integer>(leftBool || rightBool ? 1 : 0);
    }
};

// Factory initialization
Utils::Factory<std::string, InterpreterSpace::BinOperator, std::function<std::unique_ptr<InterpreterSpace::BinOperator>()>, std::shared_ptr<BaseType>, std::shared_ptr<BaseType>, std::shared_ptr<BaseType>>
        InterpreterSpace::BinOperatorFactory::factory;

void InterpreterSpace::BinOperatorFactory::initialize() {
    if (!factory.isInitialized()) {
        factory.initialize([](auto& f) {
            f.registerCreator("+", []() { return std::make_unique<AddOperator>(); });
            f.registerCreator("-", []() { return std::make_unique<SubOperator>(); });
            f.registerCreator("*", []() { return std::make_unique<MulOperator>(); });
            f.registerCreator("/", []() { return std::make_unique<DivOperator>(); });
            f.registerCreator("==", []() { return std::make_unique<EqOperator>(); });
            f.registerCreator("!=", []() { return std::make_unique<NeqOperator>(); });
            f.registerCreator(">", []() { return std::make_unique<GtOperator>(); });
            f.registerCreator("<", []() { return std::make_unique<LtOperator>(); });
            f.registerCreator(">=", []() { return std::make_unique<GteOperator>(); });
            f.registerCreator("<=", []() { return std::make_unique<LteOperator>(); });
            f.registerCreator("&", []() { return std::make_unique<AndOperator>(); });
            f.registerCreator("|", []() { return std::make_unique<OrOperator>(); });
        });
    }
}

std::shared_ptr<BaseType> InterpreterSpace::BinOperatorFactory::execute(
    const std::string& op, 
    std::shared_ptr<BaseType> left, 
    std::shared_ptr<BaseType> right
) {
    initialize();
    
    try {
        return factory.execute(op, left, right);
    } catch (const std::exception&) {
        return std::make_shared<Exception>("Unsupported operator");
    }
}

// Original visitor implementations
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