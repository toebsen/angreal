//
// Created by toebs on 05.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_
#define ANGREAL_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_

#include "../parser/ast.h"
#include "common.h"
#include "type.h"

namespace angreal::interpreter::environment {

using namespace angreal::parser;

class UnaryOP : private NonCopyable {
   public:
    UnaryOP(AST::UnaryOperation::OpType op_type, type_t type);

    type_t Call();

   private:
    type_t Add();
    type_t NegateNumeric();
    type_t NegateBool();

    AST::UnaryOperation::OpType op_type_;
    type_t type_;
};

}  // namespace angreal::interpreter::environment
#endif  // ANGREAL_SRC_INTERPRETER_ENVIRONMENT_UNARY_OP_H_
