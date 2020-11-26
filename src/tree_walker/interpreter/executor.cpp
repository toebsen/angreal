//
// Created by toebs on 14.04.2020.
//

#include "executor.h"

angreal::interpreter::Executor::Executor(
    angreal::interpreter::Interpreter& interpreter)
    : interpreter_ {interpreter} {}

angreal::interpreter::Executor::~Executor() {
    interpreter_.environment_ = original_environment_;
}

void angreal::interpreter::Executor::execute(
    const angreal::statements_t& statements,
    const std::shared_ptr<environment::Environment>& environment) {
    original_environment_ = interpreter_.environment_;
    interpreter_.environment_ = environment;
    for (const auto& stmt : statements) {
        stmt->accept(interpreter_.shared_from_this());
    }
}