//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include <../parser/ast.h>

namespace tb_lang::interpreter {

void Interpreter::visit(FunctionDeclaration* node) {}

void Interpreter::visit(Program* node) {}

void Interpreter::visit(Block* node) {}

void Interpreter::visit(Declaration* node) {
    node->expression->accept(this);
    environment_.Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(Assignment* node) {
    node->expression->accept(this);
    environment_.Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(IdentifierLiteral* node) {
    auto o = environment_.Get(node->name);
    if (o) {
        scope_.Stack().push(o);
    } else {
        throw std::runtime_error("Identfier <" + node->name +
                                 "> does not exist");
    }
}

void Interpreter::visit(IntLiteral* node) {
    type_t type = std::make_shared<IntType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(BoolLiteral* node) {
    type_t type = std::make_shared<BoolType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(FloatLiteral* node) {
    type_t type = std::make_shared<FloatType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(StringLiteral* node) {
    type_t type = std::make_shared<StringType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(UnaryOperation* node) {}

void Interpreter::visit(BinaryOperation* node) {
    node->lhs->accept(this);
    node->rhs->accept(this);

    if (node->type == BinaryOperation::OpType::Add) {
        auto a = scope_.Stack().top();
        scope_.Stack().pop();

        auto b = scope_.Stack().top();
        scope_.Stack().pop();

        auto type = binary_ops::add_op(a->GetType(), b->GetType());
        obj_t o = std::make_shared<Object>(type);
        scope_.Stack().push(o);
    }
}

void Interpreter::visit(FunctionCall* node) {}

void Interpreter::visit(FormalParameter* node) {}
}  // namespace tb_lang::interpreter