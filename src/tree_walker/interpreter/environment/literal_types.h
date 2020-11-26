//
// Created by toebs on 04.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
#define ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_

#include "../parser/ast.h"
#include "common.h"
#include "type.h"

namespace angreal::interpreter::environment {

class IntType : public Type {
   public:
    explicit IntType(int value);
};

class FloatType : public Type {
   public:
    explicit FloatType(float value);
};

class StringType : public Type {
   public:
    explicit StringType(angreal::string_t value);
};

class BoolType : public Type {
   public:
    explicit BoolType(bool value);
};

}  // namespace angreal::interpreter::environment
#endif  // ANGREAL_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
