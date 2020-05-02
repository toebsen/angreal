//
// Created by toebs on 11.04.2020.
//
#include <string>
#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, HelloWorld) {
    std::string code = R"(
    # this is a comment
    def Hello(name : string) : string {
        # this is a function
        return "Hello " + name
    }
    # this is a nested function call
    print(Hello("World"));
    )";
    SafeRun(code, "Hello World");
}


TEST_F(BaseTest, ScopingWithFunction) {
    std::string code = R"(
    var x: string = "global";
    {
        def printVar() : int {
            print(x);
            return 0;
        }

        printVar();
        var x: string = "local";
        printVar();
    }
    )";
    SafeRun(code, "global\nglobal");
}

TEST_F(BaseTest, ScopingWithFunction2) {
    std::string code = R"(
    var x: string = "global";
    {
        def printVar(x: string) : int {
            print(x);
            return 0;
        }

        printVar(x);
        var x: string = "local";
        printVar(x);
    }
    )";
    SafeRun(code, "global\nlocal");
}


TEST_F(BaseTest, SameNameWithInLocalScope) {
    std::string code = R"(
    # redefinition is allowed on a global scope
    var a : string = "first";
    var a : string = "second";
    def bad() : int {
      # but not in a local scope
      var b : string = "first";
      var b : string = "second";
    }
    )";
    ExpectRuntimeException(code, "variable `b` already declared in this scope");
}

TEST_F(BaseTest, ReturnTesting) {
    std::string code = R"(
    def somefun() : int {
        def inner() : int {
            var a : int = 5;
            return a;
        }

      var x: int = inner();
      return x;
      return 42;
    }

    var result: int = somefun();
    print(result);
    )";
    SafeRun(code, "5");
}

TEST_F(BaseTest, Conditonals) {
    std::string code = R"(
    var x: int = 5;
    if(x > 5){
        print("true");
    } else {
        print("false");
    }
    )";
    SafeRun(code, "false");
}