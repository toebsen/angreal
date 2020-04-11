//
// Created by toebs on 29.03.2020.
//

#include "interpreter.h"

#include "../parser/ast.h"
#include "../parser/parser.h"
#include "../lexer/lexer.h"
#include "environment/binary_op.h"
#include "environment/callable.h"
#include "environment/unary_op.h"

namespace tb_lang::interpreter {

void Interpreter::visit(std::shared_ptr<Program> node) {
    for (const auto& stmt : node->statements) {
        stmt->accept(shared_from_this());
    }
}

void Interpreter::visit(std::shared_ptr<Block> node) {
    for (const auto& stmt : node->statements) {
        stmt->accept(shared_from_this());
    }
}

void Interpreter::visit(std::shared_ptr<Declaration> node) {
    node->expression->accept(shared_from_this());
    environment_->Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(std::shared_ptr<Assignment> node) {
    node->expression->accept(shared_from_this());
    environment_->Declare(node->identifier, scope_.Stack().top());
    scope_.Stack().pop();
}

void Interpreter::visit(std::shared_ptr<Return> node) {
    node->expression->accept(shared_from_this());
}

void Interpreter::visit(std::shared_ptr<IdentifierLiteral> node) {
    auto o = environment_->Get(node->name);
    if (o) {
        scope_.Stack().push(o);
    } else {
        throw std::runtime_error("Identifier <" + node->name +
                                 "> does not exist");
    }
}

void Interpreter::visit(std::shared_ptr<IntLiteral> node) {
    type_t type = std::make_shared<IntType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<BoolLiteral> node) {
    type_t type = std::make_shared<BoolType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<FloatLiteral> node) {
    type_t type = std::make_shared<FloatType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<StringLiteral> node) {
    type_t type = std::make_shared<StringType>(node->value);
    obj_t o = std::make_shared<Object>(type);
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<UnaryOperation> node) {
    node->expression->accept(shared_from_this());
    auto a = scope_.Stack().top();
    scope_.Stack().pop();

    UnaryOP op(node->type, a->GetType());
    obj_t o = std::make_shared<Object>(op.Call());
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<BinaryOperation> node) {
    node->lhs->accept(shared_from_this());
    auto a = scope_.Stack().top();
    scope_.Stack().pop();

    node->rhs->accept(shared_from_this());
    auto b = scope_.Stack().top();
    scope_.Stack().pop();

    BinaryOp op(node->type, a->GetType(), b->GetType());
    obj_t o = std::make_shared<Object>(op.Call());
    scope_.Stack().push(o);
}

void Interpreter::visit(std::shared_ptr<FunctionCall> node) {
    auto callable = environment_->Get(node->identifier);
    if (callable->GetType()->IsCallable()) {

        auto fun = callable->GetType()->AsCallable();

        std::vector<obj_t> args;
        for (const auto& item : node->args) {
            item->accept(shared_from_this());
            args.push_back(scope_.Stack().top());
            scope_.Stack().pop();
        }

        auto ret_obj = fun->Call(this, args);
        if (ret_obj) {
            if (!ret_obj->GetType()->HasSameType(*fun->ReturnType())) {
                throw std::runtime_error(node->identifier +
                                         "returns wrong Type");
            }
            if (!ret_obj->GetType()->IsNull()) {
                scope_.Stack().push(ret_obj);
            }
        }
    }
}

void Interpreter::visit(std::shared_ptr<FormalParameter> node) {
    //
}

void Interpreter::visit(std::shared_ptr<FunctionDeclaration> node) {
    auto fun_decl = std::make_shared<Function>(node, environment_);
    auto type = std::make_shared<Type>(fun_decl);
    obj_t o = std::make_shared<Object>(type);
    environment_->Declare(node->identifier, o);
}

obj_t Interpreter::invoke(
    statements_t statements,
    const std::shared_ptr<environment::Environment>& env) {
    auto orig_env = env;
    try {
        environment_ = env;
        for (auto& stmt : statements) {
            stmt->accept(shared_from_this());
        }
    } catch (...) {
        environment_ = orig_env;
        throw;
    }
    environment_ = orig_env;

    auto return_value = scope_.Stack().top();
    scope_.Stack().pop();

    return return_value;
}

void Interpreter::interpret(const string_t& code) {
    lex::Lexer lexer;
    parser::Parser parser;
    try
    {
        auto lexemes = lexer.lex(code);
        auto program =  parser.parseProgram(lexemes);
        program->accept(shared_from_this());
    }
    catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }

}
void Interpreter::visit(std::shared_ptr<Print> node) {
    for (const auto& stmt : node->expressions) {
        stmt->accept(shared_from_this());
        auto val = scope_.Stack().top();
        std::cout << val->GetType()->Stringify() << std::endl;;
        scope_.Stack().pop();
    }
}

}  // namespace tb_lang::interpreter