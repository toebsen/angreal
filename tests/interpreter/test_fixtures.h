//
// Created by toebs on 30.03.2020.
//

#ifndef TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_
#define TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "interpreter/interpreter.h"
#include "parser/ast.h"

using namespace tb_lang;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;

class BaseTest : public ::testing::Test {
   public:
    BaseTest() : interpreter(scope) {}

   protected:
    Scope scope;
    Interpreter interpreter;
};

class DeclarationTest : public BaseTest {
   public:

    AST::Declaration DeclareString(const std::string& name, const std::string& value)
    {
        return {
            TypeSystem::Type::String, name,
            std::make_shared<StringLiteral>(value)};
    }

    AST::Declaration DeclareInt(const std::string& name, int value)
    {
        return {
            TypeSystem::Type::Int, name,
            std::make_shared<IntLiteral>(value)};
    }

    AST::Declaration DeclareBool(const std::string& name, bool value)
    {
        return {
            TypeSystem::Type::Bool, name,
            std::make_shared<BoolLiteral>(value)};
    }

    AST::Declaration DeclareFloat(const std::string& name, float value)
    {
        return {
            TypeSystem::Type::Float, name,
            std::make_shared<FloatLiteral>(value)};
    }

};

#endif  // TBLANG_TESTS_INTERPRETER_TEST_FIXTURES_H_
