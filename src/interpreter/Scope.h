//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_

#include <string>
#include <stack>

#include <unordered_map>
namespace tb_lang::interpreter {

class Scope
{
public:
  std::string value(const std::string& name);
  void defineValue(const std::string& name, const std::string& value);

  std::stack<std::string>& Stack();

protected:
  std::unordered_map<std::string, std::string> lut_;
  std::stack<std::string> stack_;
};

}

#endif//TBLANG_SRC_VIRTUAL_MACHINE_SCOPE_H_
