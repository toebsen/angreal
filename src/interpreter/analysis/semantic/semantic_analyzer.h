//
// Created by toebs on 18.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_
#define TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_

#include "visitor.h"

namespace tb_lang {

namespace interpreter {
namespace analysis {
class SemanticAnalyzer : public Visitor {
   public:
    SemanticAnalyzer() = default;

    TESTABLE:

    void visit(std::shared_ptr<Program> node) override;
    void visit(std::shared_ptr<Block> node) override;
    void visit(std::shared_ptr<ExpressionStatement> node) override;
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
};
}  // namespace analysis
}  // namespace interpreter
}  // namespace tb_lang

#endif  // TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_SEMANTIC_ANALYZER_H_