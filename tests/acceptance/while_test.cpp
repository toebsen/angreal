//
// Created by toebs on 13.05.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ConditionalTest, SimpleWhile) {
    std::string code = R"(
    var x = 0
    while(x < 1)
    {
        print(x)
        x = x + 1
    }
    )";
    SafeRun(code, "0");
}

TEST_F(ConditionalTest, SimpleWhile2) {
    std::string code = R"(
    while(false)
    {
        print("false")
    }
    print("end")
    )";
    SafeRun(code, R"("end")");
}