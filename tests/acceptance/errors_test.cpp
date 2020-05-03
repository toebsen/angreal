//
// Created by toebs on 03.05.2020.
//


#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, IdentifierNotDeclared)
{
    std::string code = R"(
        var x = "123";
        print(y);
    )";

    ExpectRuntimeException(code, "<y> is not defined!");
}

TEST_F(BaseTest, BinOpNotCompatibleType)
{
    std::string code = R"(
        var x = "123";
        var y = 123;
        print(x+y);
    )";
    ExpectRuntimeException(code, "Not able to execute: \"123\" <Add> 123");
}

TEST_F(BaseTest, SameNameWithInLocalScope) {
    std::string code = R"(
    # redefinition is allowed on a global scope
    var a = "first";
    var a = "second";
    def bad() {
      # but not in a local scope
      var b = "first";
      var b = "second";
    }
    )";
    ExpectRuntimeException(code, "variable `b` already declared in this scope");
}

