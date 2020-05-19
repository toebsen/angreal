//
// Created by toebs on 18.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_
#define ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_

#include "ast_interfaces.h"
#include "resolver.h"
#include "visitor.h"

namespace angreal::interpreter {
class Interpreter;

namespace analysis {

class SemanticAnalyzer : public Visitor {
   public:
    explicit SemanticAnalyzer(Interpreter& interpreter);

    void Resolve(const expression_t& expr);
    void Resolve(const expressions_t& expressions);
    void Resolve(const statement_t& statement);
    void Resolve(const statements_t& statements);
    void Resolve(const block_t& statements);

    void visit(const std::shared_ptr<Program>& node) override;
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
    void visit(const std::shared_ptr<ClassDeclaration>& node) override;
    void visit(const std::shared_ptr<Get>& node) override;
    void visit(const std::shared_ptr<Set>& node) override;
    void visit(const std::shared_ptr<Self>& node) override;

    void ResolveLocal(const node_t& node, long long int distance);

   private:
    Resolver resolver_;
    Interpreter& interpreter_;
};
}  // namespace analysis
}  // namespace angreal::interpreter

#endif  // ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_
