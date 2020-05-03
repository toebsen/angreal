//
// Created by toebs on 11.04.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, ReturnTesting) {
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

TEST_F(BaseTest, TopLevelReturn) {
    std::string code = R"(
    var result = "123";
    return result;
    )";
    ExpectRuntimeException(code, "Can not return from top level code!");
}
