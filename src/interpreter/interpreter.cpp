//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include <../parser/ast.h>

namespace tb_lang::interpreter {

void Interpreter::visit(FunctionDeclaration *node) {}

void Interpreter::visit(Program *node) {}

void Interpreter::visit(Block *node) {}

void Interpreter::visit(Declaration *node) {
    node->expression->accept(this);
    vm_.Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(Assignment *node) {
    node->expression->accept(this);
    vm_.Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(IdentifierLiteral *node) {
    scope_.Stack().push(vm_.Get(node->name));
}

void Interpreter::visit(BoolLiteral *node) {
    vm_.Declare("_", node->value);
    scope_.Stack().push(vm_.Get("_"));
}

void Interpreter::visit(IntLiteral *node) {
    vm_.Declare("_", node->value);
    scope_.Stack().push(vm_.Get("_"));
}

void Interpreter::visit(FloatLiteral *node) {
    vm_.Declare("_", node->value);
    scope_.Stack().push(vm_.Get("_"));
}

void Interpreter::visit(StringLiteral *node) {
    vm_.Declare("_", node->value);
    scope_.Stack().push(vm_.Get("_"));
}

void Interpreter::visit(UnaryOperation *node) {}

void Interpreter::visit(BinaryOperation *node) {}

void Interpreter::visit(FunctionCall *node) {}

void Interpreter::visit(FormalParameter *node) {}
}  // namespace tb_lang::interpreter