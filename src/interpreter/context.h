//
// Created by toebs on 05.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_CONTEXT_H_
#define TBLANG_SRC_INTERPRETER_CONTEXT_H_

#include "interpreter.h"
#include "scope.h"

namespace tb_lang::interpreter {

class Context {
   public:
    Context() { interpreter = std::make_shared<Interpreter>(global, env); }

    Scope global;
    Environment env;
    std::shared_ptr<Interpreter> interpreter;
};
}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_INTERPRETER_CONTEXT_H_
