/**
 * @file src/interpreter/operators.cpp
 * @brief Visitor implementation for operators.
**/

#include "interpreter.hpp"


namespace DemoLang {

// binary operator factory
class BinOperatorFactory {
private:
    static std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(std::shared_ptr<BaseType>, std::shared_ptr<BaseType>)>> operators;
    
    // Helper functions
    static bool isNumeric(std::shared_ptr<BaseType> operand);
    static std::shared_ptr<BaseType> toFloat(std::shared_ptr<BaseType> operand);
    static std::shared_ptr<BaseType> toInt(std::shared_ptr<BaseType> operand);
    static bool toBool(std::shared_ptr<BaseType> operand);
    
public:
    static void initialize();
    static std::shared_ptr<BaseType> execute(const std::string& op, std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right);
};

// Static operator map
std::unordered_map<std::string, std::function<std::shared_ptr<BaseType>(std::shared_ptr<BaseType>, std::shared_ptr<BaseType>)>> 
    BinOperatorFactory::operators;

// Helper functions
bool BinOperatorFactory::isNumeric(std::shared_ptr<BaseType> operand) {
    return operand->getName() == "Integer" || operand->getName() == "Float";
}

std::shared_ptr<BaseType> BinOperatorFactory::toInt(std::shared_ptr<BaseType> operand) {
    if (!operand) return std::make_shared<Exception>("Null operand");
    
    if (operand->getName() == "Integer") return operand;
    if (operand->getName() == "Float") {
        return std::make_shared<Integer>(static_cast<long long>(std::any_cast<long double>(operand->getValue())));
    }
    return std::make_shared<Exception>("Type conversion error");
}

std::shared_ptr<BaseType> BinOperatorFactory::toFloat(std::shared_ptr<BaseType> operand) {
    if (!operand) return std::make_shared<Exception>("Null operand");
    
    if (operand->getName() == "Float") return operand;
    if (operand->getName() == "Integer") {
        return std::make_shared<Float>(static_cast<long double>(std::any_cast<long long>(operand->getValue())));
    }
    return std::make_shared<Exception>("Type conversion error");
}

bool BinOperatorFactory::toBool(std::shared_ptr<BaseType> operand) {
    if (isNumeric(operand)) {
        auto val = toFloat(operand);
        return std::any_cast<long double>(val->getValue()) != 0.0;
    }
    if (operand->getName() == "String") {
        return !std::any_cast<std::string>(operand->getValue()).empty();
    }
    return false;
}

// Initialize all operators
void BinOperatorFactory::initialize() {
    if (!operators.empty()) return;
    
    // Arithmetic operators
    operators["+"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (left->getName() == "String" && right->getName() == "String") {
            return std::make_shared<String>(
                std::any_cast<std::string>(left->getValue()) + 
                std::any_cast<std::string>(right->getValue())
            );
        }
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        
        // Return Integer if both operands are Integer, otherwise Float
        if (left->getName() == "Integer" && right->getName() == "Integer") {
            auto lVal = std::any_cast<long long>(left->getValue());
            auto rVal = std::any_cast<long long>(right->getValue());
            return std::make_shared<Integer>(lVal + rVal);
        } else {
            auto l = toFloat(left), r = toFloat(right);
            return std::make_shared<Float>(
                std::any_cast<long double>(l->getValue()) + 
                std::any_cast<long double>(r->getValue())
            );
        }
    };
    
    operators["-"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        
        // Return Integer if both operands are Integer, otherwise Float
        if (left->getName() == "Integer" && right->getName() == "Integer") {
            auto lVal = std::any_cast<long long>(left->getValue());
            auto rVal = std::any_cast<long long>(right->getValue());
            return std::make_shared<Integer>(lVal - rVal);
        } else {
            auto l = toFloat(left), r = toFloat(right);
            return std::make_shared<Float>(
                std::any_cast<long double>(l->getValue()) - 
                std::any_cast<long double>(r->getValue())
            );
        }
    };
    
    operators["*"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        
        // Return Integer if both operands are Integer, otherwise Float
        if (left->getName() == "Integer" && right->getName() == "Integer") {
            auto lVal = std::any_cast<long long>(left->getValue());
            auto rVal = std::any_cast<long long>(right->getValue());
            return std::make_shared<Integer>(lVal * rVal);
        } else {
            auto l = toFloat(left), r = toFloat(right);
            return std::make_shared<Float>(
                std::any_cast<long double>(l->getValue()) * 
                std::any_cast<long double>(r->getValue())
            );
        }
    };
    
    operators["/"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto r = toFloat(right);
        auto rVal = std::any_cast<long double>(r->getValue());
        if (rVal == 0.0) return std::make_shared<Exception>("Division by zero");
        
        auto l = toFloat(left);
        return std::make_shared<Float>(
            std::any_cast<long double>(l->getValue()) / rVal
        );
    };
    
    // Comparison operators
    operators["=="] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (left->getName() == "String" && right->getName() == "String") {
            return std::make_shared<Integer>(
                std::any_cast<std::string>(left->getValue()) == 
                std::any_cast<std::string>(right->getValue()) ? 1 : 0
            );
        }
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) == 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    operators["!="] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (left->getName() == "String" && right->getName() == "String") {
            return std::make_shared<Integer>(
                std::any_cast<std::string>(left->getValue()) != 
                std::any_cast<std::string>(right->getValue()) ? 1 : 0
            );
        }
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) != 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    operators[">"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) > 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    operators["<"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) < 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    operators[">="] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) >= 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    operators["<="] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
        auto l = toFloat(left), r = toFloat(right);
        return std::make_shared<Integer>(
            std::any_cast<long double>(l->getValue()) <= 
            std::any_cast<long double>(r->getValue()) ? 1 : 0
        );
    };
    
    // Logical operators
    operators["&"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        return std::make_shared<Integer>(toBool(left) && toBool(right) ? 1 : 0);
    };
    
    operators["|"] = [](auto left, auto right) -> std::shared_ptr<BaseType> {
        return std::make_shared<Integer>(toBool(left) || toBool(right) ? 1 : 0);
    };
}

// Execute operator
std::shared_ptr<BaseType> BinOperatorFactory::execute(
    const std::string& op, 
    std::shared_ptr<BaseType> left, 
    std::shared_ptr<BaseType> right
) {
    initialize();
    auto it = operators.find(op);
    if (it != operators.end()) {
        return it->second(left, right);
    }
    return std::make_shared<Exception>("Unsupported operator");
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