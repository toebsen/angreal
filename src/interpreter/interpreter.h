//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_

#include <boost/algorithm/string.hpp>

#include "visitor.h"
#include "Scope.h"

namespace tb_lang::interpreter {

class Interpreter : Visitor
{
public:

  explicit Interpreter(Scope& scope)
  : scope_(scope)
  {}

  void visit(Program *node) override;
  void visit(Block *node) override;
  void visit(Declaration *node) override;
  void visit(Assignment *node) override;
  void visit(IdentifierLiteral *node) override;
  void visit(ValueLiteral<int> *node) override;
  void visit(ValueLiteral<float> *node) override;
  void visit(ValueLiteral<bool> *node) override;
  void visit(ValueLiteral<std::string> *node) override;
  void visit(BoolLiteral *node) override;
  void visit(IntLiteral *node) override;
  void visit(FloatLiteral *node) override;
  void visit(StringLiteral *node) override;
  void visit(UnaryOperation *node) override;
  void visit(BinaryOperation *node) override;
  void visit(FunctionCall *node) override;
  void visit(FormalParameter *node) override;
  void visit(FunctionDeclaration *node) override;

protected:
  Scope& scope_;
};

}


#endif//TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
