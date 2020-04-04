//
// Created by toebs on 29.03.2020.
//

#include "Scope.h"
namespace tb_lang::interpreter {
std::stack<obj_t>& Scope::Stack() { return stack_; }
}  // namespace tb_lang::interpreter