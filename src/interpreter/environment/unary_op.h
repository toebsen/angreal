//
// Created by toebs on 05.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_
#define TBLANG_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_

#include "../parser/ast.h"
#include "common.h"
#include "type.h"

namespace tb_lang::interpreter::environment {

using namespace tb_lang::parser;

class UnaryOP : private NonCopyable {
   public:
    UnaryOP(AST::UnaryOperation::OpType op_type, const type_t& type);

    type_t Call();

   private:
    type_t Add();
    type_t NegateNumeric();
    type_t NegateBool();

    AST::UnaryOperation::OpType op_type_;
    type_t type_;
};

}  // namespace tb_lang::interpreter::environment
#endif  // TBLANG_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_
