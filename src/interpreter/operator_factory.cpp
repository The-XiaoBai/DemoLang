/**
 * @file src/interpreter/operator_factory.cpp
 * @brief Operator factory pattern implementation.
 * @author The-XiaoBai
 * @date 2026/01/31
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
    if (!operand) {
        throw std::runtime_error("Null operand provided");
    }
    if (operand->getName() == "Integer") {
        return std::any_cast<long long>(operand->getValue());
    }
    throw std::runtime_error("Cannot convert non-integer to integer");
};

auto toFloat = [](std::shared_ptr<BaseType> operand) -> long double {
    // Safely convert operand to floating-point value
    if (!operand) {
        throw std::runtime_error("Null operand provided");
    }
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
            try {
                std::string leftStr = std::any_cast<std::string>(left->getValue());
                std::string rightStr = std::any_cast<std::string>(right->getValue());
                return std::make_shared<String>(leftStr + rightStr);
            } catch (const std::bad_any_cast& e) {
                return std::make_shared<Exception>("String type conversion error: " + std::string(e.what()));
            }
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
        // Support both numeric and string comparison
        if (left->getName() == "String" && right->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            return std::make_shared<Integer>(leftStr == rightStr ? 1 : 0);
        }
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be both numeric or both strings");
        }
        return std::make_shared<Integer>(toFloat(left) == toFloat(right) ? 1 : 0);
    }
};

class NeqOperator : public InterpreterSpace::BinOperator {
public:
    std::shared_ptr<BaseType> execute(std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) override {
        // Support both numeric and string comparison
        if (left->getName() == "String" && right->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            return std::make_shared<Integer>(leftStr != rightStr ? 1 : 0);
        }
        else if (!isNumeric(left) || !isNumeric(right)) {
            return std::make_shared<Exception>("Operands must be both numeric or both strings");
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
        // Support both numeric and string types for logical operations
        bool leftBool = false, rightBool = false;
        
        if (isNumeric(left)) {
            leftBool = toFloat(left) != 0.0;
        } else if (left->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            leftBool = !leftStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical AND");
        }
        
        if (isNumeric(right)) {
            rightBool = toFloat(right) != 0.0;
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
        // Support both numeric and string types for logical operations
        bool leftBool = false, rightBool = false;
        
        if (isNumeric(left)) {
            leftBool = toFloat(left) != 0.0;
        } else if (left->getName() == "String") {
            std::string leftStr = std::any_cast<std::string>(left->getValue());
            leftBool = !leftStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical OR");
        }
        
        if (isNumeric(right)) {
            rightBool = toFloat(right) != 0.0;
        } else if (right->getName() == "String") {
            std::string rightStr = std::any_cast<std::string>(right->getValue());
            rightBool = !rightStr.empty();
        } else {
            return std::make_shared<Exception>("Unsupported operand type for logical OR");
        }
        
        return std::make_shared<Integer>(leftBool || rightBool ? 1 : 0);
    }
};


// Static member initialization
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

} // namespace DemoLang