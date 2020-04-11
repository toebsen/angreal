//
// Created by toebs on 30.03.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(DeclarationTest, Test) {

//    def helloWorld() : string
//    {
//        var text : string = "World";
//        return text;
//    }

    statements_t statements{
        std::make_shared<AST::Declaration>(TypeSystem::Type::String, "text",
                                                               std::make_shared<StringLiteral>("World")),
    std::make_shared<AST::Return>(std::make_shared<AST::IdentifierLiteral>("text"))};


    formal_parameters parameters = {};
    auto declaration = std::make_shared<FunctionDeclaration>(TypeSystem::Type::String, "helloWorld", parameters, statements);
    context_.interpreter->visit(declaration);


    auto call = std::make_shared<FunctionCall>("helloWorld", expressions_t());
    context_.interpreter->visit(call);

    ASSERT_EQ(GetResultType()->AsString(), "World");
}