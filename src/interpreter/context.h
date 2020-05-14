#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-non-private-member-variables-in-classes"
//
// Created by toebs on 05.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_CONTEXT_H_
#define TBLANG_SRC_INTERPRETER_CONTEXT_H_

#include "environment/environment.h"
#include "interpreter.h"

using tb_lang::interpreter::environment::Environment;

namespace tb_lang::interpreter {

class Context {
   public:
    Context() {
        global = std::make_shared<Environment>();
        interpreter = std::make_shared<Interpreter>(global);
    }
    std::shared_ptr<Environment> global;
    std::shared_ptr<Interpreter> interpreter;
};
}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_INTERPRETER_CONTEXT_H_

#pragma clang diagnostic pop