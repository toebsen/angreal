//
// Created by toebs on 14.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_EXECUTOR_H_
#define TBLANG_SRC_INTERPRETER_EXECUTOR_H_

#include "interpreter.h"

namespace tb_lang::interpreter {

namespace environment {
class Environment;
}

class Executor : public NonCopyable {
   public:
    Executor(Interpreter& interpreter) : interpreter_{interpreter} {}

    ~Executor() { interpreter_.environment_ = original_environment_; }

    void execute(const statements_t& statements,
                 const std::shared_ptr<environment::Environment>& environment) {
        original_environment_ = interpreter_.environment_;
        interpreter_.environment_ = environment;
        for (const auto& stmt : statements) {
            stmt->accept(interpreter_.shared_from_this());
        }
    }

   private:
    Interpreter& interpreter_;
    std::shared_ptr<environment::Environment> original_environment_;
};
}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_INTERPRETER_EXECUTOR_H_
