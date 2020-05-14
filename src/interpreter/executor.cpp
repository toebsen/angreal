//
// Created by toebs on 14.04.2020.
//

#include "executor.h"

tb_lang::interpreter::Executor::Executor(
    tb_lang::interpreter::Interpreter& interpreter)
    : interpreter_{interpreter} {}

tb_lang::interpreter::Executor::~Executor() {
    interpreter_.environment_ = original_environment_;
}

void tb_lang::interpreter::Executor::execute(
    const tb_lang::statements_t& statements,
    const std::shared_ptr<environment::Environment>& environment) {
    original_environment_ = interpreter_.environment_;
    interpreter_.environment_ = environment;
    for (const auto& stmt : statements) {
        stmt->accept(interpreter_.shared_from_this());
    }
}