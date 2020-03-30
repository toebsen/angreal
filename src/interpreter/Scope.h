//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_

#include <stack>
#include <string>
#include <unordered_map>

#include <any>

namespace tb_lang::interpreter {

class Scope {
   public:
    const std::any& Value(const std::string &name);
    void DefineValue(const std::string &name, const std::any &value);

    std::stack<std::any> &Stack();

   protected:
    std::unordered_map<std::string, std::any> lut_;
    std::stack<std::any> stack_;
};

}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
