//
// Created by toebs on 18.04.2020.
//
#include "test_fixtures.h"

TEST_F(SemanticAnalyzerTest, Test)
{
    semantic_analyzer_.visit(std::make_shared<AST::IntLiteral>(1));

    ASSERT_FALSE(true);

}