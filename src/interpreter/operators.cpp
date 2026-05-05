/**
 * @file src/interpreter/operators.cpp
 * @brief Visitor implementation for operators.
**/

#include "interpreter.hpp"

using namespace DemoLang::ValueTypes;
using namespace DemoLang::AST;
using namespace DemoLang::Utils;


namespace DemoLang {

// Operator registry type
using OpRegistry = Registry<std::string, std::shared_ptr<BaseType>, std::shared_ptr<BaseType>, std::shared_ptr<BaseType>>;

// Helper functions
static bool isNumeric(std::shared_ptr<BaseType> operand) {
    return operand->getName() == "Integer" || operand->getName() == "Float";
}

static std::shared_ptr<BaseType> toFloat(std::shared_ptr<BaseType> operand) {
    if (operand->getName() == "Float") return operand;
    if (operand->getName() == "Integer") {
        return std::make_shared<Float>(static_cast<long double>(std::any_cast<long long>(operand->getValue())));
    }
    return std::make_shared<Exception>("Type conversion error");
}

static bool toBool(std::shared_ptr<BaseType> operand) {
    if (isNumeric(operand)) {
        auto val = toFloat(operand);
        return std::any_cast<long double>(val->getValue()) != 0.0;
    }
    if (operand->getName() == "String") {
        return !std::any_cast<std::string>(operand->getValue()).empty();
    }
    return false;
}

// Generic arithmetic: + - *
static std::shared_ptr<BaseType> arithmetic(
    std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right,
    std::function<long long(long long, long long)> intOp,
    std::function<long double(long double, long double)> floatOp)
{
    if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
    if (left->getName() == "Integer" && right->getName() == "Integer") {
        return std::make_shared<Integer>(
            intOp(std::any_cast<long long>(left->getValue()), std::any_cast<long long>(right->getValue())));
    }
    auto l = toFloat(left), r = toFloat(right);
    return std::make_shared<Float>(
        floatOp(std::any_cast<long double>(l->getValue()), std::any_cast<long double>(r->getValue())));
}

// Generic comparison: == != > < >= <=
static std::shared_ptr<BaseType> compare(
    std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right,
    std::function<bool(long double, long double)> cmp,
    std::function<bool(const std::string&, const std::string&)> strCmp = nullptr)
{
    if (strCmp && left->getName() == "String" && right->getName() == "String") {
        return std::make_shared<Integer>(
            strCmp(std::any_cast<std::string>(left->getValue()), std::any_cast<std::string>(right->getValue())) ? 1 : 0);
    }
    if (!isNumeric(left) || !isNumeric(right)) return std::make_shared<Exception>("Type error");
    auto l = toFloat(left), r = toFloat(right);
    return std::make_shared<Integer>(
        cmp(std::any_cast<long double>(l->getValue()), std::any_cast<long double>(r->getValue())) ? 1 : 0);
}

// Initialize operators into registry
static void initOperators() {
    auto& reg = OpRegistry::instance();
    if (reg.size() > 0) return;

    // Arithmetic: + - *
    reg.registerFunc("+", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        if (l->getName() == "String" && r->getName() == "String") {
            return std::make_shared<String>(std::any_cast<std::string>(l->getValue()) + std::any_cast<std::string>(r->getValue()));
        }
        return arithmetic(l, r, [](long long a, long long b) { return a + b; }, [](long double a, long double b) { return a + b; });
    });
    reg.registerFunc("-", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        return arithmetic(l, r, [](long long a, long long b) { return a - b; }, [](long double a, long double b) { return a - b; });
    });
    reg.registerFunc("*", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        return arithmetic(l, r, [](long long a, long long b) { return a * b; }, [](long double a, long double b) { return a * b; });
    });
    reg.registerFunc("/", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        if (!isNumeric(l) || !isNumeric(r)) return std::make_shared<Exception>("Type error");
        auto rVal = std::any_cast<long double>(toFloat(r)->getValue());
        if (rVal == 0.0) return std::make_shared<Exception>("Division by zero");
        auto lVal = std::any_cast<long double>(toFloat(l)->getValue());
        return std::make_shared<Float>(lVal / rVal);
    });

    // Comparison: == != > < >= <=
    reg.registerFunc("==", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        return compare(l, r, [](long double a, long double b) { return a == b; }, [](const std::string& a, const std::string& b) { return a == b; });
    });
    reg.registerFunc("!=", [](auto l, auto r) -> std::shared_ptr<BaseType> {
        return compare(l, r, [](long double a, long double b) { return a != b; }, [](const std::string& a, const std::string& b) { return a != b; });
    });
    reg.registerFunc(">", [](auto l, auto r) -> std::shared_ptr<BaseType> { return compare(l, r, [](long double a, long double b) { return a > b; }); });
    reg.registerFunc("<", [](auto l, auto r) -> std::shared_ptr<BaseType> { return compare(l, r, [](long double a, long double b) { return a < b; }); });
    reg.registerFunc(">=", [](auto l, auto r) -> std::shared_ptr<BaseType> { return compare(l, r, [](long double a, long double b) { return a >= b; }); });
    reg.registerFunc("<=", [](auto l, auto r) -> std::shared_ptr<BaseType> { return compare(l, r, [](long double a, long double b) { return a <= b; }); });

    // Logical: & |
    reg.registerFunc("&", [](auto l, auto r) -> std::shared_ptr<BaseType> { return std::make_shared<Integer>(toBool(l) && toBool(r) ? 1 : 0); });
    reg.registerFunc("|", [](auto l, auto r) -> std::shared_ptr<BaseType> { return std::make_shared<Integer>(toBool(l) || toBool(r) ? 1 : 0); });
}

// Execute operator via registry
static std::shared_ptr<BaseType> executeOp(const std::string& op, std::shared_ptr<BaseType> left, std::shared_ptr<BaseType> right) {
    initOperators();
    auto& reg = OpRegistry::instance();
    if (reg.isRegistered(op)) {
        return reg.execute(op, left, right);
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
            // Check if right side is an anonymous function (lambda)
            if (auto* funcDef = dynamic_cast<FunctionDefNode*>(node.getRight())) {
                if (funcDef->isAnonymous()) {
                    env.setFunction(name, std::make_shared<FunctionDefNode>(*funcDef));
                    result = std::make_shared<String>("[function]");
                } else {
                    result = std::make_shared<Exception>("Cannot assign named function");
                }
            } else {
                env.set(name, *right);
                result = right;
            }
        } else {
            result = std::make_shared<Exception>("Left side of assignment must be an identifier");
        }
        return;
    }
    
    // For all other binary operators, use registry
    result = executeOp(node.getOp(), left, right);
}

} // namespace DemoLang