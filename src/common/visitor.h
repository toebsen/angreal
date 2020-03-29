//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_VISITOR_H
#define TBLANG_VISITOR_H

#include <string>

namespace tb_lang {

namespace parser::AST {
  class Program;
  class Block;
  class Declaration;
  class Assignment;
  class IdentifierLiteral;
  template<typename ValueType> class ValueLiteral;
  class BoolLiteral;
  class IntLiteral;
  class FloatLiteral;
  class StringLiteral;
  class UnaryOperation;
  class BinaryOperation;
  class FunctionCall;
  class FormalParameter;
  class FunctionDeclaration;
}// namespace parser::AST

using namespace parser::AST;

class Visitor
{
public:
  virtual void visit(Program *node) = 0;
  virtual void visit(Block *node) = 0;
  virtual void visit(Declaration *node) = 0;
  virtual void visit(Assignment *node) = 0;
  virtual void visit(IdentifierLiteral *node) = 0;
  virtual void visit(ValueLiteral<int> *node) = 0;
  virtual void visit(ValueLiteral<float> *node) = 0;
  virtual void visit(ValueLiteral<bool> *node) = 0;
  virtual void visit(ValueLiteral<std::string> *node) = 0;
  virtual void visit(BoolLiteral *node) = 0;
  virtual void visit(IntLiteral *node) = 0;
  virtual void visit(FloatLiteral *node) = 0;
  virtual void visit(StringLiteral *node) = 0;
  virtual void visit(UnaryOperation *node) = 0;
  virtual void visit(BinaryOperation *node) = 0;
  virtual void visit(FunctionCall *node) = 0;
  virtual void visit(FormalParameter *node) = 0;
  virtual void visit(FunctionDeclaration *node) = 0;
};
}// namespace tb_lang

#endif//TBLANG_VISITOR_H
