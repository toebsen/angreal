//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, BinaryOp) {
    DeclareInt("a", 1);
    DeclareInt("b", 2);

    auto bin_op = BinaryOperation("+", std::make_shared<IdentifierLiteral>("a"),
                                  std::make_shared<IdentifierLiteral>("b"));
    context_.interpreter->visit(&bin_op);

    ASSERT_EQ(3, GetResultType()->AsInteger());
}


TEST_F(FloatBinaryOpTest, PureFloat)
{
    DeclareBinaryOp("+", "a", "b");
    EXPECT_NEAR(15.0, GetResultType()->AsFloat(), 1e6);

    DeclareBinaryOp("-", "a", "b");
    EXPECT_NEAR(5.0, GetResultType()->AsFloat(), 1e6);

    DeclareBinaryOp("/", "a", "b");
    EXPECT_NEAR(2.0, GetResultType()->AsFloat(), 1e6);

    DeclareBinaryOp("*", "a", "b");
    EXPECT_NEAR(50.0, GetResultType()->AsFloat(), 1e6);

    DeclareBinaryOp("==", "a", "b");
    EXPECT_EQ(false, GetResultType()->AsBoolean() );

    DeclareBinaryOp("!=", "a", "b");
    EXPECT_EQ(true, GetResultType()->AsBoolean());
}

TEST_F(IntBinaryOpTest, PureInt)
{
    DeclareBinaryOp("+", "a", "b");
    EXPECT_NEAR(15, GetResultType()->AsInteger(), 1e6);

    DeclareBinaryOp("-", "a", "b");
    EXPECT_NEAR(5, GetResultType()->AsInteger(), 1e6);

    DeclareBinaryOp("/", "a", "b");
    EXPECT_NEAR(2, GetResultType()->AsInteger(), 1e6);

    DeclareBinaryOp("*", "a", "b");
    EXPECT_NEAR(50, GetResultType()->AsInteger(), 1e6);

    DeclareBinaryOp("==", "a", "b");
    EXPECT_EQ(false, GetResultType()->AsBoolean() );

    DeclareBinaryOp("!=", "a", "b");
    EXPECT_EQ(true, GetResultType()->AsBoolean());
}

TEST_F(BooleanBinaryOpTest, PureBool)
{
    DeclareBinaryOp("or", "a", "b");
    EXPECT_EQ(true, GetResultType()->AsBoolean() );

    DeclareBinaryOp("and", "a", "b");
    EXPECT_EQ(false, GetResultType()->AsBoolean());

    DeclareBinaryOp("==", "a", "b");
    EXPECT_EQ(false, GetResultType()->AsBoolean() );

    DeclareBinaryOp("!=", "a", "b");
    EXPECT_EQ(true, GetResultType()->AsBoolean());
}

TEST_F(StringBinaryOpTest, PureString)
{
    DeclareBinaryOp("+", "a", "b");
    EXPECT_EQ("ab", GetResultType()->AsString() );

    DeclareBinaryOp("==", "a", "b");
    EXPECT_EQ(false, GetResultType()->AsBoolean() );

    DeclareBinaryOp("!=", "a", "b");
    EXPECT_EQ(true, GetResultType()->AsBoolean());
}