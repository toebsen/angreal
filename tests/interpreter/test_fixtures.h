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
        auto obj = context_.global.Stack().top();
        return obj->GetType();
    }

    type_t GetResultType(const std::string& name) {
        auto obj = context_.env.Get(name);
        return obj->GetType();
    }

    void DeclareString(const std::string& name, const std::string& value) {
        AST::Declaration decl{TypeSystem::Type::String, name,
                              std::make_shared<StringLiteral>(value)};
        context_.interpreter->visit(&decl);
    }

    void DeclareInt(const std::string& name, int value) {
        AST::Declaration decl{TypeSystem::Type::Int, name,
                              std::make_shared<IntLiteral>(value)};
        context_.interpreter->visit(&decl);
    }

    void DeclareBool(const std::string& name, bool value) {
        AST::Declaration decl{TypeSystem::Type::Bool, name,
                              std::make_shared<BoolLiteral>(value)};
        context_.interpreter->visit(&decl);
    }

    void DeclareFloat(const std::string& name, float value) {
        AST::Declaration decl{TypeSystem::Type::Float, name,
                              std::make_shared<FloatLiteral>(value)};
        context_.interpreter->visit(&decl);
    }
};

class UnaryOpTest : public DeclarationTest
{
   protected:
    void DeclareUnaryOp(const std::string& op, const std::string& lit)
    {
        auto unary_op =
            UnaryOperation(op, std::make_shared<IdentifierLiteral>(lit));
        context_.interpreter->visit(&unary_op);
    }
};

class BinaryOpTest : public DeclarationTest {
   protected:
    void DeclareBinaryOp(const std::string op, const std::string lhs,
                         const std::string rhs) {
        auto bin_op =
            BinaryOperation(op, std::make_shared<IdentifierLiteral>(lhs),
                            std::make_shared<IdentifierLiteral>(rhs));

        context_.interpreter->visit(&bin_op);
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

#endif  // TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_
