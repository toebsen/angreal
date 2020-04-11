//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Assignment) {
    DeclareString("test", "Hello");
    ASSERT_EQ(GetResultType("test")->AsString(), "Hello");

    auto assignment = std::make_shared<AST::Assignment>("test",
                               std::make_shared<StringLiteral>("Hello World!"));
    context_.interpreter->visit(assignment);

    ASSERT_EQ(GetResultType("test")->AsString(), "Hello World!");
}
