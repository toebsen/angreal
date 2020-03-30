//
// Created by toebs on 29.03.2020.
//

#include "Scope.h"

void tb_lang::interpreter::Scope::DefineValue(const std::string &name,
                                              const std::any &value) {
    lut_[name] = value;
}

std::stack<std::any> &tb_lang::interpreter::Scope::Stack() { return stack_; }

const std::any &tb_lang::interpreter::Scope::Value(
    const std::string &name) {
    return lut_[name];
}
