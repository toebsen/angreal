//
// Created by toebs on 29.03.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"


TEST_F(DeclarationTest, String) {
    AST::Declaration declaration{DeclareString("test", "Hello World!")};
    interpreter.visit(&declaration);
    auto obj = vm.Get("test");
    auto type = std::dynamic_pointer_cast<StringType>(obj->GetType());
    ASSERT_EQ(type->Value(), "Hello World!");
}

TEST_F(DeclarationTest, Int) {
    AST::Declaration declaration{DeclareInt("test", 42)};
    interpreter.visit(&declaration);
    auto obj = vm.Get("test");
    auto type = std::dynamic_pointer_cast<IntType>(obj->GetType());
    ASSERT_EQ(type->Value(), 42);
}

TEST_F(DeclarationTest, Float) {
    AST::Declaration declaration{DeclareFloat("test", 4.2)};
    interpreter.visit(&declaration);
    auto obj = vm.Get("test");
    auto type = std::dynamic_pointer_cast<FloatType>(obj->GetType());
    ASSERT_NEAR(type->Value(), 4.2, 1e-6);
}

TEST_F(DeclarationTest, Bool) {
    AST::Declaration declaration{DeclareBool("test", false)};
    interpreter.visit(&declaration);
    auto obj = vm.Get("test");
    auto type = std::dynamic_pointer_cast<BoolType>(obj->GetType());
    ASSERT_EQ(type->Value(), false);
}