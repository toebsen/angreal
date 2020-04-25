//
// Created by toebs on 30.03.2020.
//

#ifndef TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_
#define TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "context.h"
#include "parser/ast.h"

using namespace tb_lang;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;
using namespace tb_lang::interpreter::environment;

class BaseTest : public ::testing::Test {
   public:
   protected:
    Context context_;
};

class DeclarationTest : public BaseTest {
   public:
    type_t GetResultType() {
        auto obj = context_.interpreter->Stack().top();
        return obj->GetType();
    }

    type_t GetResultType(const std::string& name) {
        auto obj = context_.global->Get(name);
        return obj->GetType();
    }

    void DeclareString(const std::string& name, const std::string& value) {
        auto decl = std::make_shared<AST::Declaration>(TypeSystem::Type::String, name,
                              std::make_shared<StringLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareInt(const std::string& name, int value) {
        auto decl = std::make_shared<AST::Declaration>(TypeSystem::Type::Int, name,
                              std::make_shared<IntLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareBool(const std::string& name, bool value) {
        auto decl = std::make_shared<AST::Declaration>(TypeSystem::Type::Bool, name,
                              std::make_shared<BoolLiteral>(value));
        context_.interpreter->visit(decl);
    }

    void DeclareFloat(const std::string& name, float value) {
        auto decl = std::make_shared<AST::Declaration>(TypeSystem::Type::Float, name,
                              std::make_shared<FloatLiteral>(value));
        context_.interpreter->visit(decl);
    }
};

class UnaryOpTest : public DeclarationTest
{
   protected:
    void DeclareUnaryOp(const std::string& op, const std::string& lit)
    {
        auto unary_op = std::make_shared<UnaryOperation>(op, std::make_shared<IdentifierLiteral>(lit));
        context_.interpreter->visit(unary_op);
    }
};

class BinaryOpTest : public DeclarationTest {
   protected:
    void DeclareBinaryOp(const std::string op, const std::string lhs,
                         const std::string rhs) {
        auto bin_op =
            std::make_shared<BinaryOperation>(op, std::make_shared<IdentifierLiteral>(lhs),
                            std::make_shared<IdentifierLiteral>(rhs));

        context_.interpreter->visit(bin_op);
    }
};

class FloatBinaryOpTest : public BinaryOpTest {
   protected:
    FloatBinaryOpTest() {
        DeclareFloat("a", 10.0);
        DeclareFloat("b", -5.0);
    }
};

class IntBinaryOpTest : public BinaryOpTest {
   protected:
    IntBinaryOpTest() {
        DeclareInt("a", 10);
        DeclareInt("b", -5);
    }
};

class BooleanBinaryOpTest : public BinaryOpTest {
   protected:
    BooleanBinaryOpTest() {
        DeclareBool("a", false);
        DeclareBool("b", true);
    }
};

class StringBinaryOpTest : public BinaryOpTest {
   protected:
    StringBinaryOpTest() {
        DeclareString("a", "a");
        DeclareString("b", "b");
    }
};

class FunctionTest : public DeclarationTest
{
   protected:
    void DeclareArityZeroFunction(const std::string& name, TypeSystem::Type return_type = TypeSystem::Type::String)
    {
        auto inner_decl =  std::make_shared<AST::Declaration>(TypeSystem::Type::String, "text",
                                                              std::make_shared<StringLiteral>("World"));

        auto ident = std::make_shared<AST::IdentifierLiteral>("text");
        context_.interpreter->ResolveLocal(ident, 0);
        statements_t statements{
            inner_decl,
            std::make_shared<AST::Return>(ident)};

        formal_parameters parameters = {};
        auto declaration = std::make_shared<FunctionDeclaration>(return_type, name, parameters, statements);
        context_.interpreter->visit(declaration);
    }

    void DeclareArityOneIntFunction(const std::string& name, int init_value)
    {
        auto ident = std::make_shared<AST::IdentifierLiteral>("test");
        context_.interpreter->ResolveLocal(ident, 0);

        statements_t statements{
            std::make_shared<AST::Declaration>(TypeSystem::Type::Int, "test",
                                               std::make_shared<BinaryOperation>(
                                                   "+",
                                                   std::make_shared<IntLiteral>(init_value),
                                                   std::make_shared<IdentifierLiteral>("arg1")
                                                   )
                                               ),

            std::make_shared<AST::Return>(ident)};

        formal_parameters parameters = {std::make_shared<FormalParameter>(TypeSystem::Type::Int, "arg1")};
        auto declaration = std::make_shared<FunctionDeclaration>(TypeSystem::Type::Int, name, parameters, statements);

        // Todo: run semantic analyzer
        context_.interpreter->visit(declaration);
    }
};
#endif  // TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_
