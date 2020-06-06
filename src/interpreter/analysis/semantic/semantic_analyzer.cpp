//
// Created by toebs on 18.04.2020.
//

#include "semantic_analyzer.h"

#include "../../interpreter.h"

namespace angreal::interpreter::analysis {

using FunctionType = Resolver::FunctionType;

SemanticAnalyzer::SemanticAnalyzer(error_handler_t error_handler,
                                   Interpreter& interpreter)
    : error_handler_(error_handler),
      resolver_(error_handler, *this),
      interpreter_(interpreter) {}

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
    resolver_.Declare(node->identifier, node);
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
    if (resolver_.IsNoFunction()) {
        error_handler_->AnalysisError("Can not return from top level code!",
                                      node);
    }

    if (node->expression) {
        if (resolver_.IsInitializer()) {
            error_handler_->AnalysisError(
                "Can not return a value from an initializer!", node);
        }
        Resolve(node->expression);
    }
}

void SemanticAnalyzer::visit(const std::shared_ptr<IdentifierLiteral>& node) {
    resolver_.CheckAlreadyDefined(node->name, node);
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
    Resolve(node->callee);
    Resolve(node->args);
}

void SemanticAnalyzer::visit(const std::shared_ptr<FormalParameter>& node) {}

void SemanticAnalyzer::visit(const std::shared_ptr<FunctionDeclaration>& node) {
    resolver_.Declare(node->identifier, angreal::parser::AST::node_t());
    resolver_.Define(node->identifier);
    resolver_.ResolveFunction(node, FunctionType::Function);
}

void SemanticAnalyzer::ResolveLocal(const node_t& node,
                                    long long int distance) {
    interpreter_.ResolveLocal(node, distance);
}

void SemanticAnalyzer::visit(const std::shared_ptr<IfStatement>& node) {
    Resolve(node->condition);
    Resolve(node->if_branch);
    if (node->else_branch) {
        Resolve(node->else_branch);
    }
}
void SemanticAnalyzer::visit(const std::shared_ptr<WhileStatement>& node) {
    Resolve(node->condition);
    Resolve(node->block);
}

void SemanticAnalyzer::visit(const std::shared_ptr<ClassDeclaration>& node) {
    resolver_.Declare(node->identifier, angreal::parser::AST::node_t());
    resolver_.Define(node->identifier);
    if (node->superclass) {
        if (node->identifier == node->superclass.value()->name) {
            error_handler_->AnalysisError(
                "Class " + node->identifier + " can not inherit from itself!",
                node);
        }
        Resolve(node->superclass.value());
    }
    resolver_.ResolveClass(node);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Get>& node) {
    Resolve(node->expression);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Set>& node) {
    Resolve(node->expression);
    Resolve(node->value);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Self>& node) {
    if (resolver_.IsNoClass()) {
        error_handler_->AnalysisError("self can only be used within classes!",
                                      node);
    }
    resolver_.ResolveLocal("self", node);
}

void SemanticAnalyzer::visit(const std::shared_ptr<Super>& node) {
    if (resolver_.IsNoClass()) {
        error_handler_->AnalysisError("super can only be used within classes!",
                                      node);
    } else if (!resolver_.IsSubClass()) {
        error_handler_->AnalysisError(
            "Cannot use super in a class without a super class!", node);
    }

    resolver_.ResolveLocal("super", node);
}

}  // namespace angreal::interpreter::analysis