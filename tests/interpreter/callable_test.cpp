//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(FunctionTest, StringFunctionWithZeroArity) {
    DeclareArityZeroFunction("helloWorld");
    auto call = std::make_shared<FunctionCall>("helloWorld", expressions_t());
    context_.interpreter->visit(call);
    ASSERT_EQ(GetResultType()->AsString(), "World");
}

TEST_F(FunctionTest, StringFunctionWithZeroArity_WrongArgs) {
    DeclareArityZeroFunction("helloWorld");
    auto args = expressions_t{{std::make_shared<IntLiteral>(5)}};
    auto call = std::make_shared<FunctionCall>("helloWorld", args);
    EXPECT_THROW(context_.interpreter->visit(call), std::runtime_error);
}

TEST_F(FunctionTest, StringFunctionWithZeroArity_WrongReturnType) {
    DeclareArityZeroFunction("helloWorld", TypeSystem::Type::Int);
    auto call = std::make_shared<FunctionCall>("helloWorld", expressions_t());
    EXPECT_THROW(context_.interpreter->visit(call);, std::runtime_error);
}

TEST_F(FunctionTest, IntFunctionWithOneArity) {
    DeclareArityOneIntFunction("myInt", 42);
    auto args = expressions_t{{std::make_shared<IntLiteral>(5)}};
    auto call = std::make_shared<FunctionCall>("myInt", args);
    context_.interpreter->visit(call);
    ASSERT_EQ(GetResultType()->AsInteger(), 47);
}

TEST_F(FunctionTest, IntFunctionWithOneArity_WrongArgumentType) {
    DeclareArityOneIntFunction("myInt", 42);
    auto args = expressions_t{{std::make_shared<StringLiteral>("5")}};
    auto call = std::make_shared<FunctionCall>("myInt", args);
    EXPECT_THROW(context_.interpreter->visit(call), std::runtime_error);
}