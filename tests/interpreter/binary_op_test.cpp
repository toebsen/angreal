//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, BinaryOp) {
    //    a = 1
    //    b = 2
    //    a + b

    auto a = DeclareInt("a", 1);
    auto b = DeclareInt("b", 2);

    interpreter.visit(&a);
    interpreter.visit(&b);

    auto bin_op = BinaryOperation("+", std::make_shared<IdentifierLiteral>("a"),
                                  std::make_shared<IdentifierLiteral>("b"));
    interpreter.visit(&bin_op);

    auto obj = scope.Stack().top();
    auto type = std::dynamic_pointer_cast<IntType>(obj->GetType());
    int val = type->Value();

    ASSERT_EQ(3, val);
}
