/**
 * @file src/interpreter/singles.cpp
 * @brief Visitor implementation for single nodes.
**/

#include "interpreter.hpp"


namespace DemoLang {

void InterpreterSpace::Interpreter::visit(FunctionCallNode& node) {
    builtins = ValueTypes::getBuiltins();

    // Check if this is a lambda immediate execution: (params){body}(args)
    if (node.getLambdaNode()) {
        auto lambdaNode = dynamic_cast<LambdaNode*>(node.getLambdaNode().get());
        if (lambdaNode) {
            // Evaluate arguments
            std::vector<std::shared_ptr<BaseType>> args;
            for (const auto& arg : node.getArgs()) {
                arg->accept(*this);
                args.push_back(result);
            }

            // Create a temporary function definition from the lambda
            auto body = std::shared_ptr<ASTNode>(lambdaNode->getBody(), [](ASTNode*){});
            auto funcDef = std::make_shared<FunctionDefNode>("", lambdaNode->getParams(), lambdaNode->getParamDefaults(), body);

            // Save current scope
            auto savedScope = env.scope;

            // Bind parameters to arguments
            const auto& params = funcDef->getParams();
            const auto& paramDefaults = funcDef->getParamDefaults();

            size_t bindCount = std::min(params.size(), args.size());
            for (size_t i = 0; i < bindCount; ++i) {
                env.set(params[i], *args[i]);
            }

            // Bind default values for missing arguments
            for (size_t i = bindCount; i < params.size(); ++i) {
                if (i < paramDefaults.size() && paramDefaults[i] != nullptr) {
                    paramDefaults[i]->accept(*this);
                    if (result && !dynamic_cast<Exception*>(result.get())) {
                        env.set(params[i], *result);
                    } else {
                        result = std::make_shared<Exception>("Failed to evaluate default value for parameter: " + params[i]);
                        env.scope = savedScope;
                        return;
                    }
                } else {
                    result = std::make_shared<Exception>("Missing required parameter: " + params[i]);
                    env.scope = savedScope;
                    return;
                }
            }

            // Execute lambda body
            funcDef->getBody()->accept(*this);

            // Restore scope
            env.scope = savedScope;
            return;
        }
    }

    // Evaluate arguments
    std::vector<std::shared_ptr<BaseType>> args;
    for (const auto& arg : node.getArgs()) {
        arg->accept(*this);
        args.push_back(result);
    }

    // Check if function is built-in
    auto it = builtins.find(node.getName());
    if (it != builtins.end()) {
        result = it->second(args);
        return;
    }

    // Check if function is user-defined
    if (env.hasFunction(node.getName())) {
        auto funcNode = env.getFunction(node.getName());
        auto funcDef = dynamic_cast<FunctionDefNode*>(funcNode.get());
        if (!funcDef) {
            result = std::make_shared<Exception>("Invalid function definition");
            return;
        }

        // Save current scope
        auto savedScope = env.scope;

        // Bind parameters to arguments (support positional binding and default values)
        const auto& params = funcDef->getParams();
        const auto& paramDefaults = funcDef->getParamDefaults();

        // Bind provided arguments
        size_t bindCount = std::min(params.size(), args.size());
        for (size_t i = 0; i < bindCount; ++i) {
            env.set(params[i], *args[i]);
        }

        // Bind default values for missing arguments
        for (size_t i = bindCount; i < params.size(); ++i) {
            if (i < paramDefaults.size() && paramDefaults[i] != nullptr) {
                // Evaluate default value expression
                paramDefaults[i]->accept(*this);
                if (result && !dynamic_cast<Exception*>(result.get())) {
                    env.set(params[i], *result);
                } else {
                    result = std::make_shared<Exception>("Failed to evaluate default value for parameter: " + params[i]);
                    env.scope = savedScope;
                    return;
                }
            } else {
                result = std::make_shared<Exception>("Missing required parameter: " + params[i]);
                env.scope = savedScope;
                return;
            }
        }

        // Execute function body
        funcDef->getBody()->accept(*this);

        // Restore scope
        env.scope = savedScope;
        return;
    }

    result = std::make_shared<Exception>("Unknown function: " + node.getName());
}

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
    result = std::make_shared<Exception>(node.getMessage());
}

void InterpreterSpace::Interpreter::visit(FunctionDefNode& node) {
    // Store function in environment
    env.setFunction(node.getName(), std::make_shared<FunctionDefNode>(node));
    result = std::make_shared<String>("");
}

void InterpreterSpace::Interpreter::visit(LambdaNode& node) {
    // Return lambda representation; immediate execution is handled by FunctionCallNode
    result = std::make_shared<String>("[lambda]");
}

void InterpreterSpace::Interpreter::visit(IfNode& node) {
    // Evaluate conditions sequentially, execute first matching body
    for (size_t i = 0; i < node.getConditions().size(); ++i) {
        node.getConditions()[i]->accept(*this);
        // Check if condition is truthy (non-zero, non-empty, non-exception)
        bool condTrue = false;
        if (auto integer = dynamic_cast<Integer*>(result.get())) {
            condTrue = (std::any_cast<long long>(integer->getValue()) != 0);
        } else if (auto flo = dynamic_cast<Float*>(result.get())) {
            condTrue = (std::any_cast<long double>(flo->getValue()) != 0.0);
        } else if (auto str = dynamic_cast<String*>(result.get())) {
            condTrue = (!std::any_cast<std::string>(str->getValue()).empty());
        }
        if (condTrue) {
            node.getBodies()[i]->accept(*this);
            return;
        }
    }
    // No condition matched, execute else body if exists
    if (node.getElseBody()) {
        node.getElseBody()->accept(*this);
    } else {
        result = std::make_shared<String>("");
    }
}

void InterpreterSpace::Interpreter::visit(WhileNode& node) {
    while (true) {
        // Evaluate condition before each iteration
        node.getCondition()->accept(*this);
        bool condTrue = false;
        
        // Same condition logic as IfNode
        if (auto integer = dynamic_cast<Integer*>(result.get())) {
            condTrue = (std::any_cast<long long>(integer->getValue()) != 0);
        } else if (auto flo = dynamic_cast<Float*>(result.get())) {
            condTrue = (std::any_cast<long double>(flo->getValue()) != 0.0);
        } else if (auto str = dynamic_cast<String*>(result.get())) {
            condTrue = (!std::any_cast<std::string>(str->getValue()).empty());
        }
        
        // Exit loop if condition is false
        if (!condTrue) {
            result = std::make_shared<String>("");
            return;
        }
        
        // Execute loop body
        node.getBody()->accept(*this);
        
        // Handle break/continue
        if (auto exc = dynamic_cast<Exception*>(result.get())) {
            auto msg = std::any_cast<std::string>(exc->getValue());
            if (msg == "__break__") {
                // Exit loop
                result = std::make_shared<String>("");
                return;
            } else if (msg == "__continue__") {
                // Continue to next iteration (will re-evaluate condition)
                result = std::make_shared<String>("");
                continue;
            }
            // Real exception - propagate
            return;
        }
    }
}

void InterpreterSpace::Interpreter::visit(BreakNode& node) {
    // Set break marker
    result = std::make_shared<Exception>("__break__");
}

void InterpreterSpace::Interpreter::visit(ContinueNode& node) {
    // Set continue marker
    result = std::make_shared<Exception>("__continue__");
}

void InterpreterSpace::Interpreter::visit(StatementSequenceNode& node) {
    std::shared_ptr<BaseType> lastResult = std::make_shared<String>("");
    
    for (const auto& stmt : node.getStatements()) {
        stmt->accept(*this);
        lastResult = result;
        
        // Check for control flow statements
        if (auto exc = dynamic_cast<Exception*>(lastResult.get())) {
            auto msg = std::any_cast<std::string>(exc->getValue());
            if (msg == "__break__" || msg == "__continue__") {
                // Propagate break/continue to parent loop
                result = lastResult;
                return;
            }
        }
    }
    
    result = lastResult;
}

} // namespace DemoLang