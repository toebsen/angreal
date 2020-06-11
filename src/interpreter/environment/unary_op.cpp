//
// Created by toebs on 05.04.2020.
//

#include "unary_op.h"

#include "literal_types.h"

namespace angreal::interpreter::environment {

UnaryOP::UnaryOP(angreal::UnaryOperation::OpType op_type,
                 angreal::interpreter::environment::type_t type)
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
    return std::visit(
        Overloaded {
            [](std::nullptr_t val) { return type_t(); },
            [](bool val) { return type_t(); },
            [](int i) -> type_t { return std::make_shared<IntType>(-1 * i); },
            [](float f) -> type_t {
                return std::make_shared<FloatType>(-1 * f);
            },
            [](const string_t s) { return type_t(); },
            [](const callable_t c) { return type_t(); },
            [](const instance_t i) { return type_t(); }},
        type_->value());
}

type_t UnaryOP::Add() {
    return std::visit(
        Overloaded {
            [](std::nullptr_t val) { return type_t(); },
            [](bool val) { return type_t(); },
            [](int i) -> type_t { return std::make_shared<IntType>(i); },
            [](float f) -> type_t { return std::make_shared<FloatType>(f); },
            [](const string_t s) { return type_t(); },
            [](const callable_t c) { return type_t(); },
            [](const instance_t i) { return type_t(); }},
        type_->value());
}

type_t UnaryOP::NegateBool() {
    return std::visit(
        Overloaded {
            [](std::nullptr_t val) { return type_t(); },
            [](bool val) -> type_t { return std::make_shared<BoolType>(!val); },
            [](int i) { return type_t(); }, [](float f) { return type_t(); },
            [](const string_t s) { return type_t(); },
            [](const callable_t c) { return type_t(); },
            [](const instance_t i) { return type_t(); }},
        type_->value());
}
}  // namespace angreal::interpreter::environment