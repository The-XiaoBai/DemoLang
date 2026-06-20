/**
 * @file src/interpreter/variables.cpp
 * @brief Visitor implementation for variable and definition nodes.
**/

#include "interpreter.hpp"

using namespace DemoLang;
using namespace DemoLang::Utils;
using namespace DemoLang::AST;
using namespace DemoLang::ValueTypes;

namespace DemoLang {
namespace InterpreterSpace {

void Interpreter::visit(IdNode& node) {
    if (env.hasFunction(node.getName())) {
        result = std::make_shared<String>("[function]");
        return;
    }
    result = env.has(node.getName()) ? env.get(node.getName())
        : std::make_shared<Exception>("Undefined variable: " + node.getName());
}

void Interpreter::visit(FunctionDefNode& node) {
    if (node.isAnonymous()) {
        result = std::make_shared<String>("[function]");
    } else {
        env.setFunction(node.getName(), std::make_shared<FunctionDefNode>(node));
        result = std::make_shared<String>("");
    }
}

static void bindParamsImpl(Environment& env,
                           const std::vector<std::string>& params,
                           const std::vector<std::shared_ptr<ASTNode>>& paramDefaults,
                           const std::vector<std::shared_ptr<BaseType>>& args,
                           std::shared_ptr<BaseType>& result,
                           Interpreter& interpreter) {
    size_t bindCount = std::min(params.size(), args.size());
    for (size_t i = 0; i < bindCount; ++i) {
        env.set(params[i], *args[i]);
    }

    for (size_t i = bindCount; i < params.size(); ++i) {
        if (i < paramDefaults.size() && paramDefaults[i] != nullptr) {
            paramDefaults[i]->accept(interpreter);
            if (result && !dynamic_cast<Exception*>(result.get())) {
                env.set(params[i], *result);
            } else {
                result = std::make_shared<Exception>("Failed to evaluate default value for parameter: " + params[i]);
                return;
            }
        } else {
            result = std::make_shared<Exception>("Missing required parameter: " + params[i]);
            return;
        }
    }
}

void Interpreter::visit(FunctionCallNode& node) {
    // Lambda immediate execution: (params){body}(args)
    if (node.getCalleeNode()) {
        auto funcDef = dynamic_cast<FunctionDefNode*>(node.getCalleeNode().get());
        if (funcDef) {
            std::vector<std::shared_ptr<BaseType>> args;
            for (const auto& arg : node.getArgs()) {
                arg->accept(*this);
                if (dynamic_cast<Exception*>(result.get())) return;
                args.push_back(result);
            }

            auto savedScope = env.scope;
            bindParamsImpl(env, funcDef->getParams(), funcDef->getParamDefaults(), args, result, *this);
            if (dynamic_cast<Exception*>(result.get())) {
                env.scope = savedScope;
                return;
            }

            funcDef->getBody()->accept(*this);
            // Without explicit '@', function returns empty string
            if (!funcDef->getHasExplicitReturn() && !dynamic_cast<Exception*>(result.get())) {
                result = std::make_shared<String>("");
            }
            env.scope = savedScope;
            return;
        }
    }

    std::vector<std::shared_ptr<BaseType>> args;
    for (const auto& arg : node.getArgs()) {
        arg->accept(*this);
        if (dynamic_cast<Exception*>(result.get())) return;
        args.push_back(result);
    }

    // Built-in function — returns nullptr if not found
    auto builtinResult = ValueTypes::getBuiltin(node.getName(), args);
    if (builtinResult) {
        result = builtinResult;
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
        bindParamsImpl(env, funcDef->getParams(), funcDef->getParamDefaults(), args, result, *this);
        if (dynamic_cast<Exception*>(result.get())) {
            env.scope = savedScope;
            return;
        }

        funcDef->getBody()->accept(*this);
        // Without explicit '@', function returns empty string
        if (!funcDef->getHasExplicitReturn() && !dynamic_cast<Exception*>(result.get())) {
            result = std::make_shared<String>("");
        }
        env.scope = savedScope;
        return;
    }

    result = std::make_shared<Exception>("Unknown function: " + node.getName());
}

} // namespace InterpreterSpace
} // namespace DemoLang
