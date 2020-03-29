//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"
#include <../parser/ast.h>

namespace tb_lang::interpreter {

void Interpreter::visit(FunctionDeclaration *node)
{
}
void Interpreter::visit(Program *node)
{
}
void Interpreter::visit(Block *node)
{
}
void Interpreter::visit(Declaration *node)
{
  node->expression->accept(this);
  scope_.defineValue(node->identifier, scope_.Stack().top());
  scope_.Stack().pop();
}

void Interpreter::visit(Assignment *node)
{
}

void Interpreter::visit(IdentifierLiteral *node)
{
}

void Interpreter::visit(ValueLiteral<int> *node)
{
}
void Interpreter::visit(ValueLiteral<float> *node)
{
}
void Interpreter::visit(ValueLiteral<bool> *node)
{
}
void Interpreter::visit(BoolLiteral *node)
{
}
void Interpreter::visit(IntLiteral *node)
{
}
void Interpreter::visit(ValueLiteral<std::string> *node)
{
  scope_.Stack().push(node->value);
}
void Interpreter::visit(FloatLiteral *node)
{
}
void Interpreter::visit(StringLiteral *node)
{
}
void Interpreter::visit(UnaryOperation *node)
{
}
void Interpreter::visit(BinaryOperation *node)
{
}
void Interpreter::visit(FunctionCall *node)
{
}
void Interpreter::visit(FormalParameter *node)
{
}
}// namespace tb_lang::Interpreter