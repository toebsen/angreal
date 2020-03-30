//
// Created by toebs on 29.03.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"


TEST_F(DeclarationTest, String) {
    AST::Declaration declaration{DeclareString("test", "Hello World!")};
    interpreter.visit(&declaration);
    ASSERT_EQ(std::any_cast<std::string>(scope.Value("test")), "Hello World!");
}

TEST_F(DeclarationTest, Int) {
    AST::Declaration declaration{DeclareInt("test", 42)};
    interpreter.visit(&declaration);
    ASSERT_EQ(std::any_cast<int>(scope.Value("test")), 42);
}

TEST_F(DeclarationTest, Float) {
    AST::Declaration declaration{DeclareFloat("test", 4.2)};
    interpreter.visit(&declaration);
    ASSERT_NEAR(std::any_cast<float>(scope.Value("test")), 4.2, 1e-6);
}

TEST_F(DeclarationTest, Bool) {
    AST::Declaration declaration{DeclareBool("test", false)};
    interpreter.visit(&declaration);
    ASSERT_EQ(std::any_cast<bool>(scope.Value("test")), false);
}