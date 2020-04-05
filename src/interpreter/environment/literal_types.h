//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_

#include "common.h"
#include "type.h"
#include "../parser/ast.h"

namespace tb_lang::interpreter::environment {


namespace binary_ops {
type_t add_op(type_t lhs, type_t rhs);
}



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
    explicit StringType(tb_lang::string_t value);
};

class BoolType : public Type {
   public:
    explicit BoolType(bool value);
};

}  // namespace tb_lang::interpreter::environment
#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
