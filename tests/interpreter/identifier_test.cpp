//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Identifier) {
    AST::Declaration declaration{DeclareString("test", "Hello World!")};
    context_.interpreter->visit(&declaration);

    AST::Declaration other_declaration{DeclareFloat("test2", 1.23)};
    context_.interpreter->visit(&other_declaration);

    AST::IdentifierLiteral literal{"test"};
    context_.interpreter->visit(&literal);
    auto obj = context_.global.Stack().top();

    auto type = std::dynamic_pointer_cast<StringType>(obj->GetType());
    ASSERT_EQ(type->Value(), "Hello World!");
}

TEST_F(DeclarationTest, IdentifierNotFound) {
    AST::IdentifierLiteral literal{"test"};
    EXPECT_THROW(context_.interpreter->visit(&literal), std::runtime_error);
}