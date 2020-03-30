//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Assignment) {
    AST::Declaration declaration{DeclareString("test", "Hello")};
    interpreter.visit(&declaration);
    ASSERT_EQ(std::any_cast<std::string>(scope.Value("test")), "Hello");

    AST::Assignment assignment{"test", std::make_shared<StringLiteral>("Hello World!")};
    interpreter.visit(&assignment);

    ASSERT_EQ(std::any_cast<std::string>(scope.Value("test")), "Hello World!");
}

