//
// Created by toebs on 29.03.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, String) {
    DeclareString("test", "Hello World!");
    ASSERT_EQ(GetResultType("test")->AsString(), "Hello World!");
}

TEST_F(DeclarationTest, Int) {
    DeclareInt("test", 42);
    ASSERT_EQ(GetResultType("test")->AsInteger(), 42);
}

TEST_F(DeclarationTest, Float) {
    DeclareFloat("test", 4.2);
    ASSERT_NEAR(GetResultType("test")->AsFloat(), 4.2, 1e-6);
}

TEST_F(DeclarationTest, Bool) {
    DeclareBool("test", false);
    ASSERT_EQ(GetResultType("test")->AsBoolean(), false);
}