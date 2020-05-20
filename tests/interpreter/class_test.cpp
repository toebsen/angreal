//
//
// Created by toebs on 17.05.2020.

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ClassTest, WhenClassCalled_ThenDefaultInitialize) {
    DeclareEmptyClass("MyClass");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    context_.interpreter->visit(call);
    ASSERT_EQ(GetResultType()->IsInstance(), true);
    ASSERT_EQ(GetResultType()->AsInstance()->Stringify(),
              "Instance of class(MyClass)");
}

TEST_F(ClassTest, WhenMethodCall_ThenMethodIsCalled) {
    DeclareSingleFunctionClass("MyClass", "myMethod", 42);

    expression_t call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    call = std::make_shared<Get>(call, "myMethod");
    call = std::make_shared<FunctionCall>(call, kNoArgs);

    context_.interpreter->visit(std::make_shared<ExpressionStatement>(call));

    ASSERT_EQ(GetResultType()->IsInteger(), true);
    ASSERT_EQ(GetResultType()->AsInteger(), 42);
}

TEST_F(ClassTest, WhenMethodReturnSelf_ThenBoundToClassInstance) {
    DeclareClassWithMethodReturningSelf("MyClass", "bound");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    auto assignment = std::make_shared<Declaration>("x", call);
    context_.interpreter->visit(assignment);

    auto getter = std::make_shared<Get>(
        std::make_shared<IdentifierLiteral>("x"), "bound");
    auto bound_call = std::make_shared<FunctionCall>(getter, kNoArgs);
    context_.interpreter->visit(bound_call);

    ASSERT_EQ(GetResultType()->IsInstance(), true);
    ASSERT_EQ(GetResultType()->AsInstance()->Stringify(),
              "Instance of class(MyClass)");
}

TEST_F(ClassTest, WhenClassHasInitializer_ThenInitializerIsCalled) {
    DeclareClassWithInitializerNoArgs("MyClass", "val", 42);

    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    auto assignment = std::make_shared<Declaration>("x", call);
    context_.interpreter->visit(assignment);

    auto getter =
        std::make_shared<Get>(std::make_shared<IdentifierLiteral>("x"), "val");
    context_.interpreter->visit(getter);

    ASSERT_EQ(GetResultType()->IsInteger(), true);
    ASSERT_EQ(GetResultType()->AsInteger(), 42);
}

TEST_F(ClassTest,
       WhenClassHasInitializerWithArgs_ThenInitializerIsCalledWithArgs) {
    DeclareClassWithInitializerSingleArg("MyClass", "val", "value");

    auto call = std::make_shared<FunctionCall>(
        last_class_, expressions_t {std::make_shared<IntLiteral>(42)});
    auto assignment = std::make_shared<Declaration>("x", call);
    context_.interpreter->visit(assignment);

    auto getter =
        std::make_shared<Get>(std::make_shared<IdentifierLiteral>("x"), "val");
    context_.interpreter->visit(getter);

    ASSERT_EQ(GetResultType()->IsInteger(), true);
    ASSERT_EQ(GetResultType()->AsInteger(), 42);
}

TEST_F(ClassTest, WhenDirectInitialiazerCall_ThenReturnInstance) {
    DeclareClassWithReturnInitializer("MyClass");

    try {
        auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
        auto assignment = std::make_shared<Declaration>("x", call);
        context_.interpreter->visit(assignment);

        auto getter = std::make_shared<Get>(
            std::make_shared<IdentifierLiteral>("x"), "init");
        auto init_call = std::make_shared<FunctionCall>(getter, kNoArgs);

        context_.interpreter->visit(init_call);

        ASSERT_EQ(GetResultType()->IsInstance(), true);
        ASSERT_EQ(GetResultType()->AsInstance()->Stringify(),
                  "Instance of class(MyClass)");

    } catch (std::exception& e) {
        FAIL() << e.what();
    }
}

TEST_F(ClassTest, WhenInitialiazerIsWithTooManyArgs_ThenRuntimeError) {
    DeclareClassWithInitializerNoArgs("MyClass", "val", 42);
    auto call = std::make_shared<FunctionCall>(
        last_class_, expressions_t {std::make_shared<IntLiteral>(42)});
    EXPECT_THROW(context_.interpreter->visit(call), RuntimeError);
}

TEST_F(ClassTest, WhenInitialiazerWithNotEnoughArgs_ThenRuntimeError) {
    DeclareClassWithInitializerSingleArg("MyClass", "val", "arg1");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    EXPECT_THROW(context_.interpreter->visit(call), RuntimeError);
}

TEST_F(ClassTest, WhenFieldIsSet_ThenFieldIsRetrievedWithGet) {
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

TEST_F(ClassTest, WhenErronousGetExpression_ThenRuntimeError) {
    auto assignment =
        std::make_shared<Declaration>("x", std::make_shared<IntLiteral>(123));
    context_.interpreter->visit(assignment);

    auto getter =
        std::make_shared<Get>(std::make_shared<IdentifierLiteral>("x"), "y");

    EXPECT_THROW(context_.interpreter->visit(getter), RuntimeError);
}

TEST_F(ClassTest, WhenErronousSetExpression_ThenRuntimeError) {
    auto assignment =
        std::make_shared<Declaration>("x", std::make_shared<IntLiteral>(123));
    context_.interpreter->visit(assignment);

    auto setter =
        std::make_shared<Set>(std::make_shared<IdentifierLiteral>("x"), "y",
                              std::make_shared<IntLiteral>(123));

    EXPECT_THROW(context_.interpreter->visit(setter), RuntimeError);
}

TEST_F(ClassTest, WhenErronousClassMethods_ThenRuntimeError) {
    DeclareSingleFunctionClass("MyClass", "myMethod", 42);

    expression_t call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    call = std::make_shared<Get>(call, "myMethod123");
    call = std::make_shared<FunctionCall>(call, kNoArgs);

    EXPECT_THROW(context_.interpreter->visit(
                     std::make_shared<ExpressionStatement>(call)),
                 RuntimeError);
}

TEST_F(ClassTest, WhenErronousSelfUsage_ThenRuntimeError) {
    EXPECT_THROW(context_.interpreter->visit(std::make_shared<Self>()),
                 RuntimeError);
}