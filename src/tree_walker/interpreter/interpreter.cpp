//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include <unordered_map>

#include "../lexer/lexer.h"
#include "../parser/parser.h"
#include "analysis/semantic/semantic_analyzer.h"
#include "environment/binary_op.h"
#include "environment/unary_op.h"
#include "executor.h"

namespace angreal::interpreter {

using namespace environment;

Interpreter::Interpreter(
    error_handler_t error_handler,
    const std::shared_ptr<environment::Environment>& global)
    : error_handler_(error_handler), globals_(global), environment_(global) {}

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

#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-exception-baseclass"
void Interpreter::visit(const std::shared_ptr<Return>& node) {
    if (node->expression) {
        node->expression->accept(shared_from_this());
        auto obj = stack_.top();
        stack_.pop();
        throw obj;
    }
    throw std::make_shared<Object>();
}
#pragma clang diagnostic pop

void Interpreter::visit(const std::shared_ptr<IdentifierLiteral>& node) {
    if (auto o = LookupVariable(node->name, node)) {
        stack_.push(o);
    } else {
        error_handler_->RuntimeError(
            "Identifier <" + node->name + "> does not exist", node);
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
    if (!o->GetType()) {
        std::stringstream ss;
        ss << "Not able to execute: ";
        ss << "<" << magic_enum::enum_name(node->type) << "> ";
        ss << a->GetType()->Stringify() << " ";
        error_handler_->RuntimeError(ss.str(), node);
    }
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

    if (!o->GetType()) {
        std::stringstream ss;
        ss << "Not able to execute: ";
        ss << a->GetType()->Stringify() << " ";
        ss << "<" << magic_enum::enum_name(node->type) << "> ";
        ss << b->GetType()->Stringify();
        error_handler_->RuntimeError(ss.str(), node);
    }
    stack_.push(o);
}

void Interpreter::visit(const std::shared_ptr<FunctionCall>& node) {
    //    std::cout << "Calling Function " << node->identifier << std::endl;
    node->callee->accept(shared_from_this());
    auto callee = stack_.top();
    stack_.pop();

    if (!callee->GetType()->IsCallable()) {
        std::stringstream ss;
        ss << "<" << callee->GetType()->Stringify() << ">: ";
        ss << "is not callable. Only functions and classes can be called!";
        error_handler_->RuntimeError(ss.str(), node);
    }

    auto fun = callee->GetType()->AsCallable();

    std::vector<obj_t> args;
    for (const auto& item : node->args) {
        item->accept(shared_from_this());
        args.push_back(stack_.top());
        stack_.pop();
    }

    auto ret_obj = fun->Call(this, args);
    if (ret_obj.has_value() && !ret_obj.value()->GetType()->IsNull()) {
        stack_.push(ret_obj.value());
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
    Executor executor {*this};
    executor.execute(statements, environment);
}

void Interpreter::invoke(const statements_t& statements,
                         const std::shared_ptr<environment::Environment>& env) {
    Executor executor {*this};
    executor.execute(statements, env);
}

void Interpreter::interpret(const string_t& code) {
    lex::Lexer lexer;

    parser::Parser parser(error_handler_);

    auto semantic_analyzer =
        std::make_shared<analysis::SemanticAnalyzer>(error_handler_, *this);

    auto lexemes = lexer.lex(code);

    auto program = parser.parseProgram(lexemes);
    if (error_handler_->HasError()) {
        error_handler_->HandleCrucialError("Error during parsing...");
        return;
    }

    semantic_analyzer->Resolve(program->statements);
    if (error_handler_->HasError()) {
        error_handler_->HandleCrucialError("Error during analysis...");
        return;
    }

    try {
        interpret(program->statements);

    } catch (const RuntimeError& e) {
        throw;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        error_handler_->RuntimeError(e.what());
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

    if (condition->GetType()->IsTruthy()) {
        node->if_branch->accept(shared_from_this());
    } else {
        if (node->else_branch) {
            node->else_branch->accept(shared_from_this());
        }
    }
}

void Interpreter::visit(const std::shared_ptr<WhileStatement>& node) {
    node->condition->accept(shared_from_this());
    auto condition = stack_.top();
    stack_.pop();
    while (condition->GetType()->IsTruthy()) {
        node->block->accept(shared_from_this());

        node->condition->accept(shared_from_this());
        condition = stack_.top();
        stack_.pop();
    }
}

void Interpreter::visit(const std::shared_ptr<ClassDeclaration>& node) {
    std::optional<obj_t> superclass {std::nullopt};
    if (node->superclass.has_value()) {
        node->superclass.value()->accept(shared_from_this());
        superclass = stack_.top();
        stack_.pop();

        const auto is_class {superclass.value()->GetType()->IsCallable() &&
                             std::dynamic_pointer_cast<Class>(
                                 superclass.value()->GetType()->AsCallable()) !=
                                 nullptr};

        if (!is_class) {
            error_handler_->RuntimeError(
                "Class <" + node->identifier +
                    "> Superclass must be a class. Not " +
                    superclass.value()->GetType()->Stringify() + "!",
                node);
        }
    }

    if (superclass) {
        environment_ = std::make_shared<Environment>(environment_);
        environment_->Declare("super", superclass.value());
    }

    std::unordered_map<string_t, obj_t> methods;
    for (const auto& method : node->methods) {
        auto method_decl = std::make_shared<Function>(method, environment_);
        auto type = std::make_shared<Type>(method_decl);
        obj_t m = std::make_shared<Object>(type);
        methods.insert_or_assign(method->identifier, m);
    }

    auto class_decl =
        std::make_shared<Class>(node, methods, superclass, environment_);

    if (superclass) {
        environment_ = environment_->Parent();
    }

    auto type = std::make_shared<Type>(class_decl);
    obj_t o = std::make_shared<Object>(type);
    environment_->Declare(node->identifier, o);
}

void Interpreter::visit(const std::shared_ptr<Get>& node) {
    node->expression->accept(shared_from_this());
    auto obj = stack_.top();
    stack_.pop();

    if (obj->GetType()->IsInstance()) {
        auto getter_obj = obj->GetType()->AsInstance()->Get(node->identifier);
        stack_.push(getter_obj);
        return;
    }

    error_handler_->RuntimeError(
        "Only instances have properties! <" + node->identifier + ">", node);
}

void Interpreter::visit(const std::shared_ptr<Set>& node) {
    node->expression->accept(shared_from_this());
    auto obj = stack_.top();
    stack_.pop();

    if (!obj->GetType()->IsInstance()) {
        error_handler_->RuntimeError("Only instances have fields", node);
    }

    node->value->accept(shared_from_this());
    auto value = stack_.top();
    stack_.pop();

    obj->GetType()->AsInstance()->Set(node->identifier, value);
}

void Interpreter::visit(const std::shared_ptr<Self>& node) {
    auto self = LookupVariable("self", node);
    stack_.push(self);
}

void Interpreter::visit(const std::shared_ptr<Super>& node) {
    auto distance = locals_.find(node);

    auto obj = LookupVariable("super", node);
    auto super = std::dynamic_pointer_cast<Class>(obj->GetType()->AsCallable());
    auto instance = environment_->Get("self", distance->second - 1);
    auto method = super->FindMethod(node->identifier);

    if (!method || !method.value()->GetType()->IsCallable()) {
        error_handler_->RuntimeError("super(" + super->Stringify() + ") of " +
                                         instance->GetType()->Stringify() +
                                         "does not have method <" +
                                         node->identifier + ">",
                                     node);
    }

    auto function = std::dynamic_pointer_cast<Function>(
        method.value()->GetType()->AsCallable());

    stack_.push(function->Bind(instance));
}

environment::obj_t Interpreter::LookupVariable(const string_t& name,
                                               const expression_t& expr) {
    if (auto distance_iter = locals_.find(expr);
        distance_iter != locals_.end()) {
        return environment_->Get(name, distance_iter->second);
    }
    return globals_->Get(name);
}
void Interpreter::ResolveLocal(const node_t& node, long long int distance) {
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

std::stack<environment::obj_t>& Interpreter::Stack() { return stack_; }

}  // namespace angreal::interpreter