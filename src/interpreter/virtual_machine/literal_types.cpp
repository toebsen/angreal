//
// Created by toebs on 04.04.2020.
//
#include "literal_types.h"

namespace tb_lang::interpreter::virtual_machine {

template <typename T>
LiteralType<T>::LiteralType(const T& value, TypeName type_name)
    : Type(type_name), value_{value} {}

template <typename T>
T LiteralType<T>::Value() const {
    return value_;
}

template <typename T>
void LiteralType<T>::setValue(const T& value) {
    value_ = value;
}

BoolType::BoolType(bool value) : LiteralType<bool>(value, TypeName::Bool) {}

IntType::IntType(int value) : LiteralType<int>(value, TypeName::Int) {}

FloatType::FloatType(float value)
    : LiteralType<float>(value, TypeName::Float) {}

StringType::StringType(tb_lang::string_t value)
    : LiteralType<tb_lang::string_t>(std::move(value), TypeName::String) {}

type_t binary_ops::add_op(type_t lhs, type_t rhs) {
    if (lhs->GetTypeName() == rhs->GetTypeName()) {
        if (lhs->GetTypeName() == Type::TypeName::Int) {
            auto l = std::dynamic_pointer_cast<IntType>(lhs);
            auto r = std::dynamic_pointer_cast<IntType>(rhs);
            return type_t(new IntType(l->Value() + r->Value()));
        }
        if (lhs->GetTypeName() == Type::TypeName::Float) {
            auto l = std::dynamic_pointer_cast<FloatType>(lhs);
            auto r = std::dynamic_pointer_cast<FloatType>(rhs);
            return type_t(new FloatType(l->Value() + r->Value()));
        }
    }
    return type_t();
}

}  // namespace tb_lang::interpreter::virtual_machine