//
// Created by toebs on 29.03.2020.
//

#include "Scope.h"
std::string tb_lang::interpreter::Scope::value(const std::string& name)
{
  return lut_[name];
}
void tb_lang::interpreter::Scope::defineValue(const std::string &name, const std::string &value)
{
  lut_[name] = value;
}

std::stack<std::string> &tb_lang::interpreter::Scope::Stack()
{
  return stack_;
}
