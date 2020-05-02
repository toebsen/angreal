//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "analysis/semantic/semantic_analyzer.h"
#include "environment/binary_op.h"
#include "environment/callable.h"
#include "environment/unary_op.h"
#include "executor.h"

namespace tb_lang::interpreter {

using namespace environment;

void Interpreter::visit(const std::shared_ptr<Program>& node) {
    for (const auto& stmt : node->statements) {
        stmt->accept(shared_from_this());
    }
}

void Interpreter::interpret(const statement_t& statement) {
    statement->accept(shared_from_this());
}

void Interpreter::interpret(const expression_t& expression) {
    expression->accept(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<Block>& node) {
    ExecuteBlock(node->statements, std::make_shared<Environment>(environment_));
}

void Interpreter::visit(const std::shared_ptr<Declaration>& node) {
    node->expression->accept(shared_from_this());
    environment_->Declare(node->identifier, stack_.top());
    stack_.pop();
}

void Interpreter::visit(const std::shared_ptr<Assignment>& node) {
    node->expression->accept(shared_from_this());
    if (locals_.contains(node)) {
        environment_->Assign(node->identifier, stack_.top(), locals_[node]);
    } else {
        globals_->Assign(node->identifier, stack_.top());
    }
    stack_.pop();
}

void Interpreter::visit(const std::shared_ptr<Return>& node) {
    node->expression->accept(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<IdentifierLiteral>& node) {
    auto o = LookupVariable(node->name, node);
    if (o) {
        stack_.push(o);
    } else {
        throw RuntimeError("Identifier <" + node->name + "> does not exist");
    }
}

void Interpreter::visit(const std::shared_ptr<IntLiteral>& node) {
    type_t type = std::make_shared<IntType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<BoolLiteral>& node) {
    type_t type = std::make_shared<BoolType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<FloatLiteral>& node) {
    type_t type = std::make_shared<FloatType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<StringLiteral>& node) {
    type_t type = std::make_shared<StringType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<UnaryOperation>& node) {
    node->expression->accept(shared_from_this());
    auto a = stack_.top();
    stack_.pop();

    UnaryOP op(node->type, a->GetType());
    obj_t o = std::make_shared<Object>(op.Call());
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<BinaryOperation>& node) {
    node->lhs->accept(shared_from_this());
    auto a = stack_.top();
    stack_.pop();

    node->rhs->accept(shared_from_this());
    auto b = stack_.top();
    stack_.pop();

    BinaryOp op(node->type, a->GetType(), b->GetType());
    obj_t o = std::make_shared<Object>(op.Call());
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<FunctionCall>& node) {
    //    std::cout << "Calling Function " << node->identifier << std::endl;
    auto callable = LookupVariable(node->identifier, node);
    if (callable->GetType()->IsCallable()) {
        auto fun = callable->GetType()->AsCallable();

        std::vector<obj_t> args;
        for (const auto& item : node->args) {
            item->accept(shared_from_this());
            args.push_back(stack_.top());
            stack_.pop();
        }

        auto ret_obj = fun->Call(this, args);
        if (ret_obj) {
            if (!ret_obj->GetType()->HasSameType(*fun->ReturnType())) {
                throw RuntimeError(node->identifier + "returns wrong Type");
            }
            if (!ret_obj->GetType()->IsNull()) {
                stack_.push(ret_obj);
            }
        }
    }
}

void Interpreter::visit(const std::shared_ptr<FormalParameter>& node) {
    //
}

void Interpreter::visit(const std::shared_ptr<FunctionDeclaration>& node) {
    //    std::cout << "Declaring Function " << node->identifier << std::endl;
    auto fun_decl = std::make_shared<Function>(node, environment_);
    auto type = std::make_shared<Type>(fun_decl);
    obj_t o = std::make_shared<Object>(type);

    environment_->Declare(node->identifier, o);
}

void Interpreter::ExecuteBlock(
    const statements_t& statements,
    const std::shared_ptr<environment::Environment>& environment) {
    Executor executor{*this};
    executor.execute(statements, environment);
}

obj_t Interpreter::invoke(
    statements_t statements,
    const std::shared_ptr<environment::Environment>& env) {
    Executor executor{*this};
    executor.execute(statements, env);

    auto return_value = stack_.top();
    stack_.pop();

    return return_value;
}

void Interpreter::interpret(const string_t& code) {
    lex::Lexer lexer;
    parser::Parser parser;
    auto semantic_analyzer =
        std::make_shared<analysis::SemanticAnalyzer>(*this);

    try {
        auto lexemes = lexer.lex(code);
        auto program = parser.parseProgram(lexemes);
        semantic_analyzer->Resolve(program->statements);
        interpret(program->statements);
    } catch (const RuntimeError& e) {
        throw;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        throw RuntimeError(e.what());
    }
}
void Interpreter::visit(const std::shared_ptr<Print>& node) {
    std::vector<obj_t> args;
    for (const auto& item : node->expressions) {
        item->accept(shared_from_this());
        args.push_back(stack_.top());
        stack_.pop();
    }

    for (const auto& arg : args) {
        std::cout << arg->GetType()->Stringify() << std::endl;
    }
}
void Interpreter::visit(const std::shared_ptr<ExpressionStatement>& node) {
    node->expression->accept(shared_from_this());
}

void Interpreter::visit(const std::shared_ptr<IfStatement>& node) {
    node->condition->accept(shared_from_this());
    auto condition = stack_.top();
    stack_.pop();

    if(condition->GetType()->IsTruthy())
    {
        node->block->accept(shared_from_this());
    }
    else
    {
        node->else_block->accept(shared_from_this());
    }
}

environment::obj_t Interpreter::LookupVariable(const string_t& name,
                                               const expression_t& expr) {
    if (auto distance_iter = locals_.find(expr);
        distance_iter != locals_.end()) {
        return environment_->Get(name, distance_iter->second);
    } else {
        return globals_->Get(name);
    }
}
void Interpreter::ResolveLocal(const node_t& node, size_t distance) {
//  std::cout << "ResolveLocal " << node << " " << distance << std::endl;
  locals_[node] = distance;
}

void Interpreter::interpret(const statements_t& statements) {
    for (const auto& statement : statements) {
        interpret(statement);
    }
}

void Interpreter::interpret(const expressions_t& expressions) {
    for (const auto& expression : expressions) {
        interpret(expressions);
    }
}


}  // namespace tb_lang::interpreter