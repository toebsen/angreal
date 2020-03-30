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
    scope_.DefineValue(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(Assignment *node) {
    node->expression->accept(this);
    scope_.DefineValue(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(IdentifierLiteral *node) {
    std::any value = scope_.Value(node->name);
    scope_.Stack().push(value);
}

void Interpreter::visit(BoolLiteral *node) {
    scope_.Stack().push(node->value);
}

void Interpreter::visit(IntLiteral *node) {
    scope_.Stack().push(node->value);
}

void Interpreter::visit(FloatLiteral *node) {
    scope_.Stack().push(node->value);
}

void Interpreter::visit(StringLiteral *node) {
    scope_.Stack().push(node->value);
}

void Interpreter::visit(UnaryOperation *node) {}

void Interpreter::visit(BinaryOperation *node) {}

void Interpreter::visit(FunctionCall *node) {}

void Interpreter::visit(FormalParameter *node) {}
}  // namespace tb_lang::interpreter