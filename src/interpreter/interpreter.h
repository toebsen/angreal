//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_

#include "scope.h"
#include "environment/environment.h"
#include "visitor.h"

namespace tb_lang::interpreter {

class Interpreter : Visitor {
   public:
    Interpreter(Scope& scope, environment::Environment& env)
        : scope_(scope), environment_(env) {}

    void visit(Program* node) override;
    void visit(Block* node) override;
    void visit(Declaration* node) override;
    void visit(Assignment* node) override;
    void visit(IdentifierLiteral* node) override;
    void visit(BoolLiteral* node) override;
    void visit(IntLiteral* node) override;
    void visit(FloatLiteral* node) override;
    void visit(StringLiteral* node) override;
    void visit(UnaryOperation* node) override;
    void visit(BinaryOperation* node) override;
    void visit(FunctionCall* node) override;
    void visit(FormalParameter* node) override;
    void visit(FunctionDeclaration* node) override;

   protected:
    Scope& scope_;
    environment::Environment& environment_;
};

}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
