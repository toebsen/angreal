//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include "../lexer/lexer.h"
#include "../parser/ast.h"
#include "../parser/parser.h"
#include "environment/binary_op.h"
#include "environment/callable.h"
#include "environment/unary_op.h"
#include "executor.h"

namespace tb_lang::interpreter {

using namespace environment;

void Interpreter::visit(std::shared_ptr<Program> node) {
    for (const auto& stmt : node->statements) {
        stmt->accept(shared_from_this());
    }
}

void Interpreter::visit(std::shared_ptr<Block> node) {
    ExecuteBlock(node->statements, std::make_shared<Environment>(environment_));
}

void Interpreter::visit(std::shared_ptr<Declaration> node) {
    //    std::cout << "Declaring Var " << node->identifier << std::endl;
    node->expression->accept(shared_from_this());
    environment_->Declare(node->identifier, stack_.top());
    stack_.pop();
}

void Interpreter::visit(std::shared_ptr<Assignment> node) {
    //    std::cout << "Assign Var " << node->identifier << std::endl;
    node->expression->accept(shared_from_this());
    environment_->Assign(node->identifier, stack_.top());
    stack_.pop();
}

void Interpreter::visit(std::shared_ptr<Return> node) {
    node->expression->accept(shared_from_this());
}

void Interpreter::visit(std::shared_ptr<IdentifierLiteral> node) {
    auto o = environment_->Get(node->name);
    if (o) {
        stack_.push(o);
    } else {
        throw RuntimeError("Identifier <" + node->name + "> does not exist");
    }
}

void Interpreter::visit(std::shared_ptr<IntLiteral> node) {
    type_t type = std::make_shared<IntType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(std::shared_ptr<BoolLiteral> node) {
    type_t type = std::make_shared<BoolType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(std::shared_ptr<FloatLiteral> node) {
    type_t type = std::make_shared<FloatType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(std::shared_ptr<StringLiteral> node) {
    type_t type = std::make_shared<StringType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    stack_.push(o);
}

void Interpreter::visit(std::shared_ptr<UnaryOperation> node) {
    node->expression->accept(shared_from_this());
    auto a = stack_.top();
    stack_.pop();

    UnaryOP op(node->type, a->GetType());
    obj_t o = std::make_shared<Object>(op.Call());
    stack_.push(o);
}

void Interpreter::visit(std::shared_ptr<BinaryOperation> node) {
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

void Interpreter::visit(std::shared_ptr<FunctionCall> node) {
    //    std::cout << "Calling Function " << node->identifier << std::endl;
    auto callable = environment_->Get(node->identifier);
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

void Interpreter::visit(std::shared_ptr<FormalParameter> node) {
    //
}

void Interpreter::visit(std::shared_ptr<FunctionDeclaration> node) {
    //    std::cout << "Declaring Function " << node->identifier << std::endl;
    auto fun_decl = std::make_shared<Function>(node, environment_);
    auto type = std::make_shared<Type>(fun_decl);
    obj_t o = std::make_shared<Object>(type);
    environment_->Declare(node->identifier, o);
}

void Interpreter::ExecuteBlock(
    statements_t statements,
    std::shared_ptr<environment::Environment> environment) {
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
    try {
        auto lexemes = lexer.lex(code);
        auto program = parser.parseProgram(lexemes);
        program->accept(shared_from_this());
    } catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
void Interpreter::visit(std::shared_ptr<Print> node) {
    for (const auto& stmt : node->expressions) {
        stmt->accept(shared_from_this());
        auto val = stack_.top();
        std::cout << val->GetType()->Stringify() << std::endl;
        ;
        stack_.pop();
    }
}
void Interpreter::visit(std::shared_ptr<ExpressionStatement> node) {
    node->expression->accept(shared_from_this());
}

}  // namespace tb_lang::interpreter