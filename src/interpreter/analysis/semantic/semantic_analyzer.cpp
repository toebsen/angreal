//
// Created by toebs on 18.04.2020.
//

#include "semantic_analyzer.h"

#include "../../interpreter.h"

namespace tb_lang::interpreter::analysis {

SemanticAnalyzer::SemanticAnalyzer(Interpreter& interpreter)
    : resolver_(*this), interpreter_(interpreter) {}

void SemanticAnalyzer::Resolve(const expression_t& expr) {
    expr->accept(shared_from_this());
}
void SemanticAnalyzer::Resolve(const expressions_t& expressions) {
    for (const auto& expression : expressions) {
        Resolve(expression);
    }
}

void SemanticAnalyzer::Resolve(const statement_t& statement) {
    statement->accept(shared_from_this());
}

void SemanticAnalyzer::Resolve(const statements_t& statements) {
    for (const auto& statement : statements) {
        Resolve(statement);
    }
}

void SemanticAnalyzer::Resolve(const block_t& block) {
    Resolve(block->statements);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Program>& node) {
    Resolve(node->statements);
}
void SemanticAnalyzer::visit(const std::shared_ptr<Block>& node) {
    resolver_.EnterScope();
    Resolve(node->statements);
    resolver_.LeaveScope();
}

void SemanticAnalyzer::visit(const std::shared_ptr<ExpressionStatement>& node) {
    Resolve(node->expression);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Declaration>& node) {
    resolver_.Declare(node->identifier);
    if (node->expression) {
        Resolve(node->expression);
    }
    resolver_.Define(node->identifier);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Assignment>& node) {
    Resolve(node->expression);
    resolver_.ResolveLocal(node->identifier, node);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Return>& node) {
    if (!resolver_.IsFunction()) {
        throw RuntimeError("Can not return from top level code!");
    }

    Resolve(node->expression);
}

void SemanticAnalyzer::visit(const std::shared_ptr<IdentifierLiteral>& node) {
    resolver_.CheckAlreadyDefined(node->name);
    resolver_.ResolveLocal(node->name, node);
}

void SemanticAnalyzer::visit(const std::shared_ptr<BoolLiteral>& node) {}
void SemanticAnalyzer::visit(const std::shared_ptr<IntLiteral>& node) {}
void SemanticAnalyzer::visit(const std::shared_ptr<FloatLiteral>& node) {}
void SemanticAnalyzer::visit(const std::shared_ptr<StringLiteral>& node) {}

void SemanticAnalyzer::visit(const std::shared_ptr<Print>& node) {
    Resolve(node->expressions);
}

void SemanticAnalyzer::visit(const std::shared_ptr<UnaryOperation>& node) {
    Resolve(node->expression);
}
void SemanticAnalyzer::visit(const std::shared_ptr<BinaryOperation>& node) {
    Resolve(node->lhs);
    Resolve(node->rhs);
}

void SemanticAnalyzer::visit(const std::shared_ptr<FunctionCall>& node) {
    Resolve(node->args);
    resolver_.ResolveLocal(node->identifier, node);
}

void SemanticAnalyzer::visit(const std::shared_ptr<FormalParameter>& node) {}

void SemanticAnalyzer::visit(const std::shared_ptr<FunctionDeclaration>& node) {
    resolver_.Declare(node->identifier);
    resolver_.Define(node->identifier);
    resolver_.ResolveFunction(node);
}

void SemanticAnalyzer::ResolveLocal(const node_t& node,
                                    long long int distance) {
    interpreter_.ResolveLocal(node, distance);
}

void SemanticAnalyzer::visit(const std::shared_ptr<IfStatement>& node) {
    Resolve(node->condition);
    Resolve(node->block);
    if (node->else_block) {
        Resolve(node->else_block);
    }
}
void SemanticAnalyzer::visit(const std::shared_ptr<WhileStatement>& node) {
    Resolve(node->condition);
    Resolve(node->block);
}

}  // namespace tb_lang::interpreter::analysis