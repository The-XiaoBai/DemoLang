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

} // namespace DemoLang