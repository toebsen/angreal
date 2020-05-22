//
// Created by toebs on 03.05.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ErroneousTest, IdentifierNotDeclared) {
    std::string code = R"(
        var x = "123"
        print(y)
    )";

    ExpectRuntimeException(code, "<y> is not defined!");
}

TEST_F(ErroneousTest, BinOpNotCompatibleType) {
    std::string code = R"(
        var x = "123"
        var y = 123
        print(x+y)
    )";
    ExpectRuntimeException(code, "Not able to execute: \"123\" <Add> 123");
}

TEST_F(ErroneousTest, UnOpNotCompatibleType) {
    std::string code = R"(
        var x = 123
        print(!x)
    )";
    ExpectRuntimeException(code, "Not able to execute: <Not> 123 ");
}

TEST_F(ErroneousTest, FunctioCallWithNonCallable) {
    std::string code = R"(
        var x = "123"
        x()
    )";
    ExpectRuntimeException(code,
                           "<\"123\">: is not callable. Only functions and "
                           "classes can be called!");
}

TEST_F(ErroneousTest, SameNameWithInLocalScope) {
    std::string code = R"(
    # redefinition is allowed on a global scope
    var a = "first"
    var a = "second"
    def bad() {
      # but not in a local scope
      var b = "first"
      var b = "second"
    }
    )";
    ExpectRuntimeException(code, "variable `b` already declared in this scope");
}

TEST_F(ErroneousTest, SelfOutsideOfMethod) {
    std::string code = R"(
    def fun()
    {
        print(self);
    }
    )";
    ExpectRuntimeException(code, "Self can only be used in bound methods!");
}

TEST_F(ErroneousTest, ReturnValueFromClassInitializer) {
    std::string code = R"(
    class InitReturn
    {
        def init()
        {
            return "123"
        }
    }
    )";
    ExpectRuntimeException(code, "Can not return a value from an initializer!");
}

TEST_F(ErroneousTest, ClassInheritingFromItself) {
    std::string code = R"(
    class BigEgo(BigEgo)
    {
    }
    )";
    ExpectRuntimeException(code, "Class BigEgo can not inherit from itself!");
}