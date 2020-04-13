//
// Created by toebs on 13.04.2020.
//

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, InnerScopeRedeclaration) {
    statements_t outer_scope;
    outer_scope.push_back(std::make_shared<AST::Declaration>(
        TypeSystem::Type::String, "a",
        std::make_shared<StringLiteral>("outer")));

    statements_t inner_scope;
    inner_scope.push_back(std::make_shared<AST::Declaration>(
        TypeSystem::Type::String, "a",
        std::make_shared<StringLiteral>("inner")));

    outer_scope.push_back(std::make_shared<Block>(inner_scope));
    auto prog = std::make_shared<Program>(outer_scope);

    context_.interpreter->visit(prog);

    ASSERT_EQ(GetResultType("a")->AsString(), "outer");
}

TEST_F(DeclarationTest, InnerScopeAssignmentToOuterScope) {
    statements_t outer_scope;
    outer_scope.push_back(std::make_shared<AST::Declaration>(
        TypeSystem::Type::String, "a",
        std::make_shared<StringLiteral>("outer")));

    statements_t inner_scope;
    inner_scope.push_back(std::make_shared<AST::Assignment>(
        "a", std::make_shared<StringLiteral>("inner")));

    outer_scope.push_back(std::make_shared<Block>(inner_scope));
    auto prog = std::make_shared<Program>(outer_scope);

    context_.interpreter->visit(prog);

    ASSERT_EQ(GetResultType("a")->AsString(), "inner");
}