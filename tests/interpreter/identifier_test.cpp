//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Identifier) {
    DeclareString("test", "Hello World!");
    DeclareFloat("test2", 1.23);

    auto literal = std::make_shared<AST::IdentifierLiteral>("test");
    context_.interpreter->visit(literal);

    ASSERT_EQ(GetResultType()->AsString(), "Hello World!");
}

TEST_F(DeclarationTest, IdentifierNotFound) {
    auto literal = std::make_shared<AST::IdentifierLiteral>("test");
    EXPECT_THROW(context_.interpreter->visit(literal), std::runtime_error);
}