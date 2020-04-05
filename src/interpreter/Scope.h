//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_

#include <any>
#include <stack>
#include <string>
#include <unordered_map>

#include "environment/environment.h"

namespace tb_lang::interpreter {
using namespace environment;

class Scope {
   public:
    std::stack<obj_t>& Stack();

   protected:
    std::unordered_map<std::string, obj_t> lut_;
    std::stack<obj_t> stack_;
};

}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
