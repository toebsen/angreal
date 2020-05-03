//
// Created by toebs on 03.05.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, HelloWorld) {
    std::string code = R"(
    # this is a comment
    def Hello(name) {
        # this is a function
        return "Hello " + name
    }
    # this is a nested function call
    print(Hello("World"));
    )";
    SafeRun(code, "\"Hello World\"");
}

TEST_F(BaseTest, Fibonacci) {
    std::string code = R"(
    def fib(n)
    {
        if(n <= 1){ return 1;}
        return fib(n-2) + fib(n-1);
    }
    print(fib(5));
    )";
    SafeRun(code, "8");
}