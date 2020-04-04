//
// Created by toebs on 04.04.2020.
//
#include "literal_types.h"

namespace tb_lang::interpreter::virtual_machine {

template <typename T>
LiteralType<T>::LiteralType(const T& value) : value_{value} {}

template <typename T>
T LiteralType<T>::Value() const {
    return value_;
}

template <typename T>
void LiteralType<T>::setValue(const T& value) {
    value_ = value;
}

BoolType::BoolType(const bool& value) : LiteralType<bool>(value) {}

IntType::IntType(const int& value) : LiteralType<int>(value) {}

FloatType::FloatType(const float& value) : LiteralType<float>(value) {}

StringType::StringType(const tb_lang::string_t& value)
    : LiteralType<tb_lang::string_t>(value) {}

}  // namespace tb_lang::interpreter::virtual_machine