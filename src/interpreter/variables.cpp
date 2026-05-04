/**
 * @file src/interpreter/variables.cpp
 * @brief Visitor implementation for variable and definition nodes.
**/

#include "interpreter.hpp"

namespace DemoLang {

void InterpreterSpace::Interpreter::visit(IdNode& node) {
    result = env.has(node.getName()) ? env.get(node.getName())
        : std::make_shared<Exception>("Undefined variable: " + node.getName());
}

void InterpreterSpace::Interpreter::visit(FunctionDefNode& node) {
    env.setFunction(node.getName(), std::make_shared<FunctionDefNode>(node));
    result = std::make_shared<String>("");
}

void InterpreterSpace::Interpreter::visit(FunctionCallNode& node) {
    builtins = ValueTypes::getBuiltins();

    // Lambda immediate execution: (params){body}(args)
    if (node.getLambdaNode()) {
        auto lambdaNode = dynamic_cast<LambdaNode*>(node.getLambdaNode().get());
        if (lambdaNode) {
            std::vector<std::shared_ptr<BaseType>> args;
            for (const auto& arg : node.getArgs()) {
                arg->accept(*this);
                args.push_back(result);
            }

            auto body = std::shared_ptr<ASTNode>(lambdaNode->getBody(), [](ASTNode*){});
            auto funcDef = std::make_shared<FunctionDefNode>("", lambdaNode->getParams(), lambdaNode->getParamDefaults(), body);

            auto savedScope = env.scope;
            const auto& params = funcDef->getParams();
            const auto& paramDefaults = funcDef->getParamDefaults();

            size_t bindCount = std::min(params.size(), args.size());
            for (size_t i = 0; i < bindCount; ++i) {
                env.set(params[i], *args[i]);
            }

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

            funcDef->getBody()->accept(*this);
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

    // Built-in function
    auto it = builtins.find(node.getName());
    if (it != builtins.end()) {
        result = it->second(args);
        return;
    }

    // User-defined function
    if (env.hasFunction(node.getName())) {
        auto funcNode = env.getFunction(node.getName());
        auto funcDef = dynamic_cast<FunctionDefNode*>(funcNode.get());
        if (!funcDef) {
            result = std::make_shared<Exception>("Invalid function definition");
            return;
        }

        auto savedScope = env.scope;
        const auto& params = funcDef->getParams();
        const auto& paramDefaults = funcDef->getParamDefaults();

        size_t bindCount = std::min(params.size(), args.size());
        for (size_t i = 0; i < bindCount; ++i) {
            env.set(params[i], *args[i]);
        }

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

        funcDef->getBody()->accept(*this);
        env.scope = savedScope;
        return;
    }

    result = std::make_shared<Exception>("Unknown function: " + node.getName());
}

void InterpreterSpace::Interpreter::visit(LambdaNode& node) {
    result = std::make_shared<String>("[lambda]");
}

} // namespace DemoLang
