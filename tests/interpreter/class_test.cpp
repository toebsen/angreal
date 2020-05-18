//
// Created by toebs on 17.05.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ClassTest, DefaultConstructor) {
    DeclareEmptyClass("MyClass");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    context_.interpreter->visit(call);
    ASSERT_EQ(GetResultType()->IsInstance(), true);
    ASSERT_EQ(GetResultType()->AsInstance()->Stringify(),
              "Instance of class(MyClass)");
}

TEST_F(ClassTest, GetSetExpression) {
    DeclareEmptyClass("MyClass");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    auto assignment = std::make_shared<Declaration>("x", call);
    context_.interpreter->visit(assignment);

    auto setter =
        std::make_shared<Set>(std::make_shared<IdentifierLiteral>("x"), "y",
                              std::make_shared<IntLiteral>(123));
    context_.interpreter->visit(setter);

    auto getter =
        std::make_shared<Get>(std::make_shared<IdentifierLiteral>("x"), "y");

    context_.interpreter->visit(getter);

    ASSERT_EQ(GetResultType()->IsInteger(), true);
    ASSERT_EQ(GetResultType()->AsInteger(), 123);
}

TEST_F(ClassTest, ErronousGetExpression) {
    auto assignment =
        std::make_shared<Declaration>("x", std::make_shared<IntLiteral>(123));
    context_.interpreter->visit(assignment);

    auto getter =
        std::make_shared<Get>(std::make_shared<IdentifierLiteral>("x"), "y");

    EXPECT_THROW(context_.interpreter->visit(getter), RuntimeError);
}

TEST_F(ClassTest, ErronousSetExpression) {
    auto assignment =
        std::make_shared<Declaration>("x", std::make_shared<IntLiteral>(123));
    context_.interpreter->visit(assignment);

    auto setter =
        std::make_shared<Set>(std::make_shared<IdentifierLiteral>("x"), "y",
                              std::make_shared<IntLiteral>(123));

    EXPECT_THROW(context_.interpreter->visit(setter), RuntimeError);
}