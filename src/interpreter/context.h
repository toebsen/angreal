//
// Created by toebs on 05.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_CONTEXT_H_
#define TBLANG_SRC_INTERPRETER_CONTEXT_H_

#include "interpreter.h"
#include "environment/environment.h"

namespace tb_lang::interpreter {

using namespace environment;

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
