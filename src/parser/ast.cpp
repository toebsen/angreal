//
// Created by bichlmaier on 07.02.2020.
//

#include "ast.h"

#include <boost/lexical_cast.hpp>

namespace angreal::parser::AST {

BoolLiteral::BoolLiteral(const std::string& value)
    : ValueLiteral<bool>(value == "true") {}

void BoolLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

IntLiteral::IntLiteral(const std::string& value)
    : ValueLiteral<int>(boost::lexical_cast<int>(value)) {}

void IntLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}
IntLiteral::IntLiteral(int value) : ValueLiteral(value) {}
TypeHelper::Type IntLiteral::type() { return TypeHelper::Type::Int; }

FloatLiteral::FloatLiteral(const std::string& value)
    : ValueLiteral<float>(boost::lexical_cast<float>(value)) {}

void FloatLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}
FloatLiteral::FloatLiteral(float value) : ValueLiteral(value) {}
TypeHelper::Type FloatLiteral::type() { return TypeHelper::Type::Float; }

StringLiteral::StringLiteral(const std::string& value)
    : ValueLiteral<std::string>(value) {}

void StringLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

TypeHelper::Type StringLiteral::type() { return TypeHelper::Type::String; }

void Program::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Program::Program(statements_t statements) : statements(std::move(statements)) {}

void Block::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Block::Block(statements_t statements) : statements(std::move(statements)) {}

void ExpressionStatement::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

ExpressionStatement::ExpressionStatement(expression_t expression)
    : expression(std::move(expression)) {}

void Declaration::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Declaration::Declaration(std::string identifier, expression_t expression)
    : identifier(std::move(identifier)), expression(std::move(expression)) {}

void Assignment::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Assignment::Assignment(std::string identifier, expression_t expression)
    : identifier(std::move(identifier)), expression(std::move(expression)) {}

void Return::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Return::Return(expression_t expression) : expression(std::move(expression)) {}

void Print::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void IdentifierLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}
bool IdentifierLiteral::operator==(const IdentifierLiteral& rhs) const {
    return name == rhs.name;
}

void UnaryOperation::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

UnaryOperation::OpType UnaryOperation::inferType(const std::string& value) {
    if (value == "+") {
        return OpType::Add;
    }
    if (value == "-") {
        return OpType::Sub;
    }
    if (value == "!") {
        return OpType::Not;
    }

    return OpType::Unknown;
}

UnaryOperation::UnaryOperation(UnaryOperation::OpType opType,
                               expression_t expression)
    : type(opType), expression(std::move(expression)) {}

UnaryOperation::UnaryOperation(const std::string& opType,
                               const expression_t& expression)
    : UnaryOperation(inferType(opType), expression) {}

void BinaryOperation::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

BinaryOperation::OpType BinaryOperation::inferType(const std::string& value) {
    if (value == "+") {
        return OpType::Add;
    }
    if (value == "-") {
        return OpType::Sub;
    }
    if (value == "*") {
        return OpType::Mul;
    }
    if (value == "/") {
        return OpType::Divide;
    }
    if (value == "or") {
        return OpType::Or;
    }
    if (value == "and") {
        return OpType::And;
    }
    if (value == "==") {
        return OpType::Equals;
    }
    if (value == "!=") {
        return OpType::NotEquals;
    }
    if (value == ">") {
        return OpType::Greater;
    }
    if (value == ">=") {
        return OpType::GreaterEquals;
    }
    if (value == "<") {
        return OpType::Less;
    }
    if (value == "<=") {
        return OpType::LessEquals;
    }

    return OpType::Unknown;
}

BinaryOperation::BinaryOperation(const std::string& opType,
                                 const expression_t& lhs,
                                 const expression_t& rhs)
    : BinaryOperation(inferType(opType), lhs, rhs) {}

BinaryOperation::BinaryOperation(BinaryOperation::OpType opType,
                                 expression_t lhs, expression_t rhs)
    : type(opType), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

void FunctionCall::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

FunctionCall::FunctionCall(const expression_t& callee,
                           const expressions_t& args)
    : callee(std::move(callee)), args(std::move(args)) {}

void FunctionDeclaration::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

FunctionDeclaration::FunctionDeclaration(std::string identifier,
                                         formal_parameters parameters,
                                         statements_t statements)
    : identifier(std::move(identifier)),
      parameters(std::move(parameters)),
      statements(std::move(statements)) {}

void FormalParameter::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

FormalParameter::FormalParameter(std::string identifier)
    : identifier(std::move(identifier)) {}

void IfStatement::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

IfStatement::IfStatement(expression_t condition, block_t if_branch,
                         block_t else_branch)
    : condition(std::move(condition)),
      if_branch(std::move(if_branch)),
      else_branch(std::move(else_branch)) {}

void WhileStatement::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

WhileStatement::WhileStatement(expression_t condition, block_t block)
    : condition(std::move(condition)), block(std::move(block)) {}

ClassDeclaration::ClassDeclaration(string_t identifier, functions_t methods)
    : identifier(std::move(identifier)), methods(std::move(methods)) {}

void ClassDeclaration::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Get::Get(expression_t expression, std::string identifier)
    : expression(std::move(expression)), identifier(std::move(identifier)) {}

void Get::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

Set::Set(expression_t expression, std::string identifier, expression_t value)
    : expression(std::move(expression)),
      identifier(std::move(identifier)),
      value(std::move(value)) {}

void Set::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void Self::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

}  // namespace angreal::parser::AST
