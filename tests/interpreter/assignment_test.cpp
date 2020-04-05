//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Assignment) {
    AST::Declaration declaration{DeclareString("test", "Hello")};
    interpreter.visit(&declaration);
    auto obj = environment_.Get("test");
    auto type = std::dynamic_pointer_cast<StringType>(obj->GetType());
    ASSERT_EQ(type->Value(), "Hello");

    AST::Assignment assignment{"test", std::make_shared<StringLiteral>("Hello World!")};
    interpreter.visit(&assignment);

    auto assigned_obj = environment_.Get("test");
    type = std::dynamic_pointer_cast<StringType>(assigned_obj->GetType());
    ASSERT_EQ(type->Value(), "Hello World!");
}

