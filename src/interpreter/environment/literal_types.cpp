//
// Created by toebs on 04.04.2020.
//
#include "literal_types.h"

namespace tb_lang::interpreter::environment {

BoolType::BoolType(bool value) : Type(value) {}

IntType::IntType(int value) : Type(value) {}

FloatType::FloatType(float value) : Type(value) {}

StringType::StringType(tb_lang::string_t value) : Type(value) {}

}  // namespace tb_lang::interpreter::environment