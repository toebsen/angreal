#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-non-private-member-variables-in-classes"
//
// Created by toebs on 05.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_CONTEXT_H_
#define ANGREAL_SRC_INTERPRETER_CONTEXT_H_

#include "environment/environment.h"
#include "error_handler/error_reporter.h"
#include "interpreter.h"

using angreal::interpreter::environment::Environment;

namespace angreal::interpreter {

class Context {
   public:
    explicit Context(
        error_handler_t error_handler = std::make_shared<ErrorReporter>()) {
        error_handler_ = error_handler;
        global = std::make_shared<Environment>();
        interpreter = std::make_shared<Interpreter>(error_handler_, global);
    }
    std::shared_ptr<Environment> global;
    std::shared_ptr<Interpreter> interpreter;
    error_handler_t error_handler_;
};
}  // namespace angreal::interpreter

#endif  // ANGREAL_SRC_INTERPRETER_CONTEXT_H_

#pragma clang diagnostic pop