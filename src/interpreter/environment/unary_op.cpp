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
    auto resulting_type = type_t();
    std::visit(
        Overloaded {[&resulting_type](std::nullptr_t val) {},
                    [&resulting_type](bool val) {},
                    [&resulting_type](int i) {
                        resulting_type = std::make_shared<IntType>(-1 * i);
                    },
                    [&resulting_type](float f) {
                        resulting_type = std::make_shared<FloatType>(-1 * f);
                    },
                    [&resulting_type](const string_t s) {},
                    [&resulting_type](const callable_t c) {},
                    [&resulting_type](const instance_t i) {}},
        type_->value());
    return resulting_type;
}

type_t UnaryOP::Add() {
    auto resulting_type = type_t();
    std::visit(Overloaded {[&resulting_type](std::nullptr_t val) {},
                           [&resulting_type](bool val) {},
                           [&resulting_type](int i) {
                               resulting_type = std::make_shared<IntType>(i);
                           },
                           [&resulting_type](float f) {
                               resulting_type = std::make_shared<FloatType>(f);
                           },
                           [&resulting_type](const string_t s) {},
                           [&resulting_type](const callable_t c) {},
                           [&resulting_type](const instance_t i) {}},
               type_->value());
    return resulting_type;
}

type_t UnaryOP::NegateBool() {
    auto resulting_type = type_t();
    std::visit(
        Overloaded {[&resulting_type](std::nullptr_t val) {},
                    [&resulting_type](bool val) {
                        resulting_type = std::make_shared<BoolType>(!val);
                    },
                    [&resulting_type](int i) {}, [&resulting_type](float f) {},
                    [&resulting_type](const string_t s) {},
                    [&resulting_type](const callable_t c) {},
                    [&resulting_type](const instance_t i) {}},
        type_->value());
    return resulting_type;
}

}  // namespace angreal::interpreter::environment