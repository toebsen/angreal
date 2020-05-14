//
// Created by toebs on 05.04.2020.
//

#include "unary_op.h"

#include "literal_types.h"

namespace tb_lang::interpreter::environment {

UnaryOP::UnaryOP(tb_lang::UnaryOperation::OpType op_type,
                 tb_lang::interpreter::environment::type_t type)
    : op_type_(op_type), type_(std::move(type)) {}

type_t UnaryOP::Call() {
    switch (op_type_) {
        case UnaryOperation::OpType::Unknown:
            break;
        case UnaryOperation::OpType::Add:
            return Add();
        case UnaryOperation::OpType::Sub:
            return NegateNumeric();
        case UnaryOperation::OpType::Not:
            return NegateBool();
    }

    return type_t();
}

type_t UnaryOP::NegateNumeric() {
    auto op = [](auto a) { return a * -1; };
    if (type_->IsInteger()) {
        return type_t(new IntType(op(type_->AsInteger())));
    }
    if (type_->IsFloat()) {
        return type_t(new FloatType(op(type_->AsFloat())));
    }
    return type_t();
}

type_t UnaryOP::Add() {
    auto op = [](auto a) { return a * +1; };
    if (type_->IsInteger()) {
        return type_t(new IntType(op(type_->AsInteger())));
    }
    if (type_->IsFloat()) {
        return type_t(new FloatType(op(type_->AsFloat())));
    }
    return type_t();
}

type_t UnaryOP::NegateBool() {
    if (type_->IsBoolean()) {
        return type_t(new BoolType(!type_->AsBoolean()));
    }
    return type_t();
}

}  // namespace tb_lang::interpreter::environment