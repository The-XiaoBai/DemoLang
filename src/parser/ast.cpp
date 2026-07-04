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
    : name(funcName), calleeNode(nullptr), args(std::move(arguments)) {}

FunctionCallNode::FunctionCallNode(std::shared_ptr<ASTNode> callee, std::vector<std::shared_ptr<ASTNode>> arguments)
    : name(""), calleeNode(std::move(callee)), args(std::move(arguments)) {}

void FunctionCallNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::string& FunctionCallNode::getName() const {
    return name;
}

std::shared_ptr<ASTNode> FunctionCallNode::getCalleeNode() const {
    return calleeNode;
}

const std::vector<std::shared_ptr<ASTNode>>& FunctionCallNode::getArgs() const {
    return args;
}

FunctionDefNode::FunctionDefNode(const std::string& funcName, std::vector<std::string> parameters,
                                 std::vector<std::shared_ptr<ASTNode>> defaults, std::shared_ptr<ASTNode> functionBody,
                                 bool explicitReturn)
    : name(funcName), params(std::move(parameters)), paramDefaults(std::move(defaults)),
      body(std::move(functionBody)), hasExplicitReturn(explicitReturn) {}

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

bool FunctionDefNode::isAnonymous() const {
    return name.empty();
}

bool FunctionDefNode::getHasExplicitReturn() const {
    return hasExplicitReturn;
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

LoopControlNode::LoopControlNode(LoopControlType t) : type(t) {}

void LoopControlNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

bool LoopControlNode::isBreak() const {
    return type == LoopControlType::Break;
}

bool LoopControlNode::isContinue() const {
    return type == LoopControlType::Continue;
}

StatementSequenceNode::StatementSequenceNode(std::vector<std::shared_ptr<ASTNode>> stmts)
    : statements(std::move(stmts)) {}

void StatementSequenceNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::vector<std::shared_ptr<ASTNode>>& StatementSequenceNode::getStatements() const {
    return statements;
}

ListNode::ListNode(std::vector<std::shared_ptr<ASTNode>> elems)
    : elements(std::move(elems)) {}

void ListNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

const std::vector<std::shared_ptr<ASTNode>>& ListNode::getElements() const {
    return elements;
}

IndexNode::IndexNode(std::shared_ptr<ASTNode> obj, std::shared_ptr<ASTNode> idx)
    : object(std::move(obj)), index(std::move(idx)) {}

void IndexNode::accept(ASTVisitor& visitor) {
    visitor.visit(*this);
}

ASTNode* IndexNode::getObject() const {
    return object.get();
}

ASTNode* IndexNode::getIndex() const {
    return index.get();
}

} // namespace AST
} // namespace DemoLang
