/**
 * @file src/interpreter/operator_factory.cpp
 * @brief Operator factory pattern implementation.
 * @author The-XiaoBai
 * @date 2026/1/2
**/

#include "interpreter.hpp"


namespace DemoLang {

// Helper functions for type checking and conversion
auto isNumeric = [](std::shared_ptr<BaseType> operand) -> bool {
    // Check if operand is a numeric type (Integer or Float)
    return operand->getName() == "Integer" || operand->getName() == "Float";
};

auto toInt = [](std::shared_ptr<BaseType> operand) -> long long {
    // Safely convert operand to integer value
    if (operand->getName() == "Integer") {
        return std::any_cast<long long>(operand->getValue());
    }
    throw std::runtime_error("Cannot convert non-integer to integer");
};

auto toFloat = [](std::shared_ptr<BaseType> operand) -> long double {
    // Safely convert operand to floating-point value
    if (operand->getName() == "Integer") {
        return static_cast<long double>(std::any_cast<long long>(operand->getValue()));
    } else if (operand->getName() == "Float") {
        return std::any_cast<long double>(operand->getValue());
    }
    throw std::runtime_error("Cannot convert non-numeric to float");
};


// Arithmetic operators
class AddOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        // String concatenation: if both operands are strings, concatenate them
        if (left->getName() == "String" && right->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            return std::make_shared<String>(leftStr + rightStr);
        } 
        // Numeric addition: both operands must be numeric or both strings
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric or both strings");
        }
        // Type promotion: if either operand is float, result is float
        if (left->getName() == "Float" || right->getName() == "Float") {
            return std::make_shared<Float>(toFloat(left) + toFloat(right));
        } else {
            // Both are integers, result is integer
            return std::make_shared<Integer>(toInt(left) + toInt(right));
        }
    }
};

class SubOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        // Subtraction only works on numeric operands
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        // Type promotion: if either operand is float, result is float
        if (left->getName() == "Float" || right->getName() == "Float") {
            return std::make_shared<Float>(toFloat(left) - toFloat(right));
        } else {
            // Both are integers, result is integer
            return std::make_shared<Integer>(toInt(left) - toInt(right));
        }
    }
};

class MulOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        // Multiplication only works on numeric operands
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        // Type promotion: if either operand is float, result is float
        if (left->getName() == "Float" || right->getName() == "Float") {
            return std::make_shared<Float>(toFloat(left) * toFloat(right));
        } else {
            // Both are integers, result is integer
            return std::make_shared<Integer>(toInt(left) * toInt(right));
        }
    }
};

class DivOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        // Division only works on numeric operands
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        long double rightVal = toFloat(right);
        // Check for division by zero
        if (rightVal == 0.0) {
            return std::make_shared<Exception>("Division by zero");
        }
        // Division always returns float (even for integer division)
        return std::make_shared<Float>(toFloat(left) / rightVal);
    }
};


// Comparison operators
class EqOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) == toFloat(right) ? 1 : 0);
    }
};

class NeqOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) != toFloat(right) ? 1 : 0);
    }
};

class GtOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) > toFloat(right) ? 1 : 0);
    }
};

class LtOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) < toFloat(right) ? 1 : 0);
    }
};

class GteOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) >= toFloat(right) ? 1 : 0);
    }
};

class LteOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) <= toFloat(right) ? 1 : 0);
    }
};


// Logical operators
class AndOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) && toFloat(right) ? 1 : 0);
    }
};

class OrOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be numeric");
        }
        return std::make_shared<Integer>(toFloat(left) || toFloat(right) ? 1 : 0);
    }
};


// Static member initialization
std::unordered_map<std::string, std::unique_ptr<InterpreterSpace::BinOperator>>
        InterpreterSpace::BinOperatorFactory::operators;

void InterpreterSpace::BinOperatorFactory::initialize() {
    if (!operators.empty()) return;
    
    operators["+"] = std::make_unique<AddOperator>();
    operators["-"] = std::make_unique<SubOperator>();
    operators["*"] = std::make_unique<MulOperator>();
    operators["/"] = std::make_unique<DivOperator>();
    operators["=="] = std::make_unique<EqOperator>();
    operators["!="] = std::make_unique<NeqOperator>();
    operators[">"] = std::make_unique<GtOperator>();
    operators["<"] = std::make_unique<LtOperator>();
    operators[">="] = std::make_unique<GteOperator>();
    operators["<="] = std::make_unique<LteOperator>();
    operators["&"] = std::make_unique<AndOperator>();
    operators["|"] = std::make_unique<OrOperator>();
}

std::shared_ptr<BaseType> InterpreterSpace::BinOperatorFactory::execute(
    const std::string& op, 
    std::shared_ptr<BaseType> left, 
    std::shared_ptr<BaseType> right
) {
    initialize();
    
    auto it = operators.find(op);
    if (it != operators.end()) return it->second->execute(left, right);
    
    return std::make_shared<Exception>("Unsupported operator");
}

} // namespace DemoLang