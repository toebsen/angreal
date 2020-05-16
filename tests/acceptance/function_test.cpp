//
// Created by toebs on 11.04.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, InlineReturn) {
    std::string code = R"(
    def foo(b)  {
        return 5 + b;
    }
    print(foo(5))
    )";
    SafeRun(code, "10");
}

TEST_F(BaseTest, ReturnOrder) {
    std::string code = R"(
    def foo()  {
        def inner()  {
            var a = 5;
            return a;
        }

      var x = inner();
      return x;
      return 42;
    }

    var result = foo();
    print(result);
    )";
    SafeRun(code, "5");
}

TEST_F(BaseTest, Currying) {
    std::string code = R"(
    def f(a)
    {
        def g(b, c)
        {
            print(a, b, c);
        }
        return g;
    }
    var fone = f(1);
    fone(2,3);
    )";
    SafeRun(code, "1\n2\n3");
}

TEST_F(BaseTest, ConcatenatedCalls) {
    std::string code = R"(
    def f(a)
    {
        def g(b, c)
        {
            print(a, b, c);
        }
        return g;
    }
    f(1)(2,3);
    )";
    SafeRun(code, "1\n2\n3");
}

TEST_F(BaseTest, TopLevelReturn) {
    std::string code = R"(
    var result = "123";
    return result;
    )";
    ExpectRuntimeException(code, "Can not return from top level code!");
}
