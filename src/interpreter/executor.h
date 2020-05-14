//
// Created by toebs on 14.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_EXECUTOR_H_
#define ANGREAL_SRC_INTERPRETER_EXECUTOR_H_

#include "interpreter.h"

namespace angreal::interpreter {

namespace environment {
class Environment;
}

class Executor : public NonCopyable {
   public:
    explicit Executor(Interpreter& interpreter);

    ~Executor() override;

    void execute(const statements_t& statements,
                 const std::shared_ptr<environment::Environment>& environment);

   private:
    Interpreter& interpreter_;
    std::shared_ptr<environment::Environment> original_environment_;
};
}  // namespace angreal::interpreter

#endif  // ANGREAL_SRC_INTERPRETER_EXECUTOR_H_
