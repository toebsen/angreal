//
// Created by toebs on 05.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ENVIRONMENT_BINARY_OP_H_
#define TBLANG_SRC_INTERPRETER_ENVIRONMENT_BINARY_OP_H_

#include "../parser/ast.h"
#include "common.h"
#include "type.h"

namespace tb_lang::interpreter::environment {

using namespace tb_lang::parser;

class BinaryOp : private NonCopyable {
   public:
    BinaryOp(AST::BinaryOperation::OpType op_type, type_t lhs, type_t rhs);

    type_t Call();

   private:
    type_t Add();

    type_t Sub();

    type_t Mul();

    type_t Divide();

    type_t Or();

    type_t And();

    type_t Equals();

    type_t NotEquals();

    AST::BinaryOperation::OpType op_type_;
    type_t lhs_;
    type_t rhs_;
};

}  // namespace tb_lang::interpreter::environment

#endif  // TBLANG_SRC_INTERPRETER_ENVIRONMENT_BINARY_OP_H_