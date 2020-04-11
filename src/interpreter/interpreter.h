//
// Created by toebs on 29.03.2020.
//

#ifndef TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
#define TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_

#include "environment/environment.h"
#include "scope.h"
#include "visitor.h"

namespace tb_lang::interpreter {

class Interpreter : public Visitor {
   public:
    Interpreter(Scope& scope, std::shared_ptr<environment::Environment> env)
        : scope_(scope), environment_(env) {}

    void visit(std::shared_ptr<Program> node) override;
    void visit(std::shared_ptr<Block> node) override;
    void visit(std::shared_ptr<Declaration> node) override;
    void visit(std::shared_ptr<Assignment> node) override;
    void visit(std::shared_ptr<Return> node) override;
    void visit(std::shared_ptr<IdentifierLiteral> node) override;
    void visit(std::shared_ptr<BoolLiteral> node) override;
    void visit(std::shared_ptr<IntLiteral> node) override;
    void visit(std::shared_ptr<FloatLiteral> node) override;
    void visit(std::shared_ptr<StringLiteral> node) override;
    void visit(std::shared_ptr<UnaryOperation> node) override;
    void visit(std::shared_ptr<BinaryOperation> node) override;
    void visit(std::shared_ptr<FunctionCall> node) override;
    void visit(std::shared_ptr<FormalParameter> node) override;
    void visit(std::shared_ptr<FunctionDeclaration> node) override;
    void visit(std::shared_ptr<Print> node) override;

    obj_t invoke(statements_t statements, const std::shared_ptr<environment::Environment>& env);

    void interpret(const string_t& code);

   protected:
    Scope& scope_;
    std::shared_ptr<environment::Environment> environment_;
};


}  // namespace tb_lang::interpreter

#endif  // TBLANG_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
