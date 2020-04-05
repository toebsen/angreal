//
// Created by toebs on 05.04.2020.
//
//
// Created by toebs on 30.03.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(UnaryOpTest, UnaryOpSub) {
    DeclareInt("a", 1);
    DeclareUnaryOp("-", "a");
    ASSERT_EQ(-1, GetResultType()->AsInteger());
}

TEST_F(UnaryOpTest, Add) {
    DeclareInt("a", 1);
    DeclareUnaryOp("+", "a");
    ASSERT_EQ(+1, GetResultType()->AsInteger());
}

TEST_F(UnaryOpTest, UnaryOpNegate) {
    DeclareBool("a", true);
    DeclareUnaryOp("!", "a");
    ASSERT_EQ(false, GetResultType()->AsBoolean());
}