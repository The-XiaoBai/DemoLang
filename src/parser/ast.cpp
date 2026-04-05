/**
 * @file src/parser/ast.cpp
 * @brief Abstract syntax tree implementation.
**/

#include "ast.hpp"

namespace DemoLang {
namespace AST {

UnaryOpNode::UnaryOpNode(std::string op, std::shared_ptr<ASTNode> operand)
    : op(std::move(op)), operand(std::move(operand)) {}

void UnaryOpNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& UnaryOpNode::getOp() const {
    return op;
}

ASTNode* UnaryOpNode::getOperand() const {
    return operand.get();
}

BinaryOpNode::BinaryOpNode(std::string op, std::shared_ptr<ASTNode> left, std::shared_ptr<ASTNode> right)
    : op(std::move(op)), left(std::move(left)), right(std::move(right)) {}

void BinaryOpNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& BinaryOpNode::getOp() const {
    return op;
}

ASTNode* BinaryOpNode::getLeft() const {
    return left.get();
}

ASTNode* BinaryOpNode::getRight() const {
    return right.get();
}

IdNode::IdNode(const std::string& id) : name(id) {}

void IdNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& IdNode::getName() const {
    return name;
}

IntNode::IntNode(long long val) : value(val) {}

void IntNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

long long IntNode::getValue() const {
    return value;
}

FloatNode::FloatNode(long double val) : value(val) {}

void FloatNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

long double FloatNode::getValue() const {
    return value;
}

StringNode::StringNode(const std::string& val) : value(val) {}

void StringNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& StringNode::getValue() const {
    return value;
}

ErrorNode::ErrorNode(const std::string& msg) : message(msg) {}

void ErrorNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& ErrorNode::getMessage() const {
    return message;
}

FunctionCallNode::FunctionCallNode(const std::string& funcName, std::vector<std::shared_ptr<ASTNode>> arguments)
    : name(funcName), lambdaNode(nullptr), args(std::move(arguments)) {}

FunctionCallNode::FunctionCallNode(std::shared_ptr<ASTNode> lambda, std::vector<std::shared_ptr<ASTNode>> arguments)
    : name(""), lambdaNode(std::move(lambda)), args(std::move(arguments)) {}

void FunctionCallNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& FunctionCallNode::getName() const {
    return name;
}

std::shared_ptr<ASTNode> FunctionCallNode::getLambdaNode() const {
    return lambdaNode;
}

const std::vector<std::shared_ptr<ASTNode>>& FunctionCallNode::getArgs() const {
    return args;
}

FunctionDefNode::FunctionDefNode(const std::string& funcName, std::vector<std::string> parameters,
                                 std::vector<std::shared_ptr<ASTNode>> defaults, std::shared_ptr<ASTNode> functionBody)
    : name(funcName), params(std::move(parameters)), paramDefaults(std::move(defaults)), body(std::move(functionBody)) {}

void FunctionDefNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& FunctionDefNode::getName() const {
    return name;
}

const std::vector<std::string>& FunctionDefNode::getParams() const {
    return params;
}

const std::vector<std::shared_ptr<ASTNode>>& FunctionDefNode::getParamDefaults() const {
    return paramDefaults;
}

ASTNode* FunctionDefNode::getBody() const {
    return body.get();
}

LambdaNode::LambdaNode(std::vector<std::string> parameters,
                       std::vector<std::shared_ptr<ASTNode>> defaults, std::shared_ptr<ASTNode> functionBody)
    : params(std::move(parameters)), paramDefaults(std::move(defaults)), body(std::move(functionBody)) {}

void LambdaNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::vector<std::string>& LambdaNode::getParams() const {
    return params;
}

const std::vector<std::shared_ptr<ASTNode>>& LambdaNode::getParamDefaults() const {
    return paramDefaults;
}

ASTNode* LambdaNode::getBody() const {
    return body.get();
}

IfNode::IfNode(std::vector<std::shared_ptr<ASTNode>> conds,
               std::vector<std::shared_ptr<ASTNode>> bodys,
               std::shared_ptr<ASTNode> elseBod)
    : conditions(std::move(conds)), bodies(std::move(bodys)), elseBody(std::move(elseBod)) {}

void IfNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::vector<std::shared_ptr<ASTNode>>& IfNode::getConditions() const {
    return conditions;
}

const std::vector<std::shared_ptr<ASTNode>>& IfNode::getBodies() const {
    return bodies;
}

ASTNode* IfNode::getElseBody() const {
    return elseBody.get();
}

WhileNode::WhileNode(std::shared_ptr<ASTNode> cond, std::shared_ptr<ASTNode> loopBody)
    : condition(std::move(cond)), body(std::move(loopBody)) {}

void WhileNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

ASTNode* WhileNode::getCondition() const {
    return condition.get();
}

ASTNode* WhileNode::getBody() const {
    return body.get();
}

BreakNode::BreakNode() {}

void BreakNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

ContinueNode::ContinueNode() {}

void ContinueNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

StatementSequenceNode::StatementSequenceNode(std::vector<std::shared_ptr<ASTNode>> stmts)
    : statements(std::move(stmts)) {}

void StatementSequenceNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::vector<std::shared_ptr<ASTNode>>& StatementSequenceNode::getStatements() const {
    return statements;
}

} // namespace AST
} // namespace DemoLang
