//
// Created by toebs on 18.04.2020.
//

#include "semantic_analyzer.h"

namespace tb_lang {
namespace interpreter {
namespace analysis {

void SemanticAnalyzer::visit(std::shared_ptr<Program> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<Block> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<ExpressionStatement> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<Declaration> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<Assignment> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<Return> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<IdentifierLiteral> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<BoolLiteral> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<Print> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<IntLiteral> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<FloatLiteral> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<StringLiteral> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<UnaryOperation> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<BinaryOperation> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<FunctionCall> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<FormalParameter> node) {}
void SemanticAnalyzer::visit(std::shared_ptr<FunctionDeclaration> node) {}

}  // namespace analysis
}  // namespace interpreter
}  // namespace tb_lang