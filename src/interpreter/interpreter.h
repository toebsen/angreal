//
// Created by toebs on 29.03.2020.
//

#ifndef ANGREAL_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
#define ANGREAL_SRC_VIRTUAL_MACHINE_INTERPRETER_H_

#include <stack>

#include "environment/environment.h"
#include "visitor.h"

namespace angreal::interpreter {

class Executor;

class Interpreter : public Visitor {
   public:
    explicit Interpreter(
        const std::shared_ptr<environment::Environment>& global);

    void interpret(const string_t& code);
    void interpret(const statement_t& statement);
    void interpret(const statements_t& statements);
    void interpret(const expression_t& expression);
    void interpret(const expressions_t& expressions);

    void invoke(const statements_t& statements,
                const std::shared_ptr<environment::Environment>& env);

    std::stack<environment::obj_t>& Stack();

    void ResolveLocal(const node_t& node, long long int distance);

    TESTABLE : void visit(const std::shared_ptr<Program>& node) override;
    void visit(const std::shared_ptr<Block>& node) override;
    void visit(const std::shared_ptr<ExpressionStatement>& node) override;
    void visit(const std::shared_ptr<Declaration>& node) override;
    void visit(const std::shared_ptr<Assignment>& node) override;
    void visit(const std::shared_ptr<Return>& node) override;
    void visit(const std::shared_ptr<IdentifierLiteral>& node) override;
    void visit(const std::shared_ptr<BoolLiteral>& node) override;
    void visit(const std::shared_ptr<IntLiteral>& node) override;
    void visit(const std::shared_ptr<FloatLiteral>& node) override;
    void visit(const std::shared_ptr<StringLiteral>& node) override;
    void visit(const std::shared_ptr<UnaryOperation>& node) override;
    void visit(const std::shared_ptr<BinaryOperation>& node) override;
    void visit(const std::shared_ptr<FunctionCall>& node) override;
    void visit(const std::shared_ptr<FormalParameter>& node) override;
    void visit(const std::shared_ptr<FunctionDeclaration>& node) override;
    void visit(const std::shared_ptr<Print>& node) override;
    void visit(const std::shared_ptr<IfStatement>& node) override;
    void visit(const std::shared_ptr<WhileStatement>& node) override;

   private:
    friend class Executor;

    void ExecuteBlock(
        const statements_t& statements,
        const std::shared_ptr<environment::Environment>& environment);

    environment::obj_t LookupVariable(const string_t& name,
                                      const expression_t& expr);

    std::stack<environment::obj_t> stack_;
    std::shared_ptr<environment::Environment> globals_;
    std::shared_ptr<environment::Environment> environment_;
    std::unordered_map<node_t, long long int, std::hash<node_t>> locals_;
};

}  // namespace angreal::interpreter

#endif  // ANGREAL_SRC_VIRTUAL_MACHINE_INTERPRETER_H_
