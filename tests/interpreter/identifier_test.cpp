//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Identifier) {
    AST::Declaration declaration{DeclareString("test", "Hello World!")};
    interpreter.visit(&declaration);

    AST::IdentifierLiteral literal{"test"};
    interpreter.visit(&literal);

    ASSERT_EQ(std::any_cast<std::string>(scope.Stack().top()), "Hello World!");
}

TEST_F(DeclarationTest, IdentifierNotFound) {
    AST::IdentifierLiteral literal{"test"};
    interpreter.visit(&literal);

    std::any val = scope.Stack().top();
    ASSERT_FALSE(val.has_value());
}