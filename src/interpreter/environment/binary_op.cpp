//
// Created by toebs on 05.04.2020.
//

#include "binary_op.h"

#include <utility>

#include "literal_types.h"

namespace tb_lang::interpreter::environment {

BinaryOp::BinaryOp(AST::BinaryOperation::OpType op_type, type_t lhs, type_t rhs)
    : op_type_(op_type), lhs_(std::move(lhs)), rhs_(std::move(rhs)) {}

type_t BinaryOp::Call() {
    switch (op_type_) {
        case BinaryOperation::OpType::Unknown:
            break;
        case BinaryOperation::OpType::Add:
            return Add();
        case BinaryOperation::OpType::Sub:
            return Sub();
        case BinaryOperation::OpType::Mul:
            return Mul();
        case BinaryOperation::OpType::Divide:
            return Divide();
        case BinaryOperation::OpType::Or:
            return Or();
        case BinaryOperation::OpType::And:
            return And();
        case BinaryOperation::OpType::Equals:
            return Equals();
        case BinaryOperation::OpType::NotEquals:
            return NotEquals();
        case BinaryOperation::OpType::Greater:
            return Greater();
        case BinaryOperation::OpType::GreaterEquals:
            return GreaterEqual();
        case BinaryOperation::OpType::Less:
            return Less();
        case BinaryOperation::OpType::LessEquals:
            return LessEqual();
    }

    return type_t();
}

type_t BinaryOp::Add() {
    auto op = [](auto a, auto b) { return a + b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new IntType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    } if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new FloatType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    } if (lhs_->IsString() && rhs_->IsString()) {
        return type_t(new StringType(op(lhs_->AsString(), rhs_->AsString())));
    }
    return type_t();
}

type_t BinaryOp::Sub() {
    auto op = [](auto a, auto b) { return a - b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new IntType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new FloatType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }

    return type_t();
}

type_t BinaryOp::Mul() {
    auto op = [](auto a, auto b) { return a * b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new IntType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new FloatType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}

type_t BinaryOp::Divide() {
    auto op = [](auto a, auto b) { return a / b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new IntType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new FloatType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}

type_t BinaryOp::Or() {
    auto op = [](auto a, auto b) { return a || b; };
    if (lhs_->IsBoolean() && rhs_->IsBoolean()) {
        return type_t(new BoolType(op(lhs_->AsBoolean(), rhs_->AsBoolean())));
    }
    return type_t();
}

type_t BinaryOp::And() {
    auto op = [](auto a, auto b) { return a && b; };
    if (lhs_->IsBoolean() && rhs_->IsBoolean()) {
        return type_t(new BoolType(op(lhs_->AsBoolean(), rhs_->AsBoolean())));
    }
    return type_t();
}

type_t BinaryOp::Equals() {
    auto op = [](auto a, auto b) { return a == b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    if (lhs_->IsString() && rhs_->IsString()) {
        return type_t(new BoolType(op(lhs_->AsString(), rhs_->AsString())));
    }
    if (lhs_->IsBoolean() && rhs_->IsBoolean()) {
        return type_t(new BoolType(op(lhs_->AsBoolean(), rhs_->AsBoolean())));
    }
    return type_t();
}  // namespace tb_lang::interpreter::environment

type_t BinaryOp::NotEquals() {
    auto op = [](auto a, auto b) { return a != b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    if (lhs_->IsString() && rhs_->IsString()) {
        return type_t(new BoolType(op(lhs_->AsString(), rhs_->AsString())));
    }
    if (lhs_->IsBoolean() && rhs_->IsBoolean()) {
        return type_t(new BoolType(op(lhs_->AsBoolean(), rhs_->AsBoolean())));
    }
    return type_t();
}

type_t BinaryOp::Greater() {
    auto op = [](auto a, auto b) { return a > b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}

type_t BinaryOp::GreaterEqual() {
    auto op = [](auto a, auto b) { return a >= b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}

type_t BinaryOp::Less() {
    auto op = [](auto a, auto b) { return a < b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}

type_t BinaryOp::LessEqual() {
    auto op = [](auto a, auto b) { return a <= b; };
    if (lhs_->IsInteger() && rhs_->IsInteger()) {
        return type_t(new BoolType(op(lhs_->AsInteger(), rhs_->AsInteger())));
    }
    if (lhs_->IsFloat() && rhs_->IsFloat()) {
        return type_t(new BoolType(op(lhs_->AsFloat(), rhs_->AsFloat())));
    }
    return type_t();
}


}  // namespace tb_lang::interpreter::environment