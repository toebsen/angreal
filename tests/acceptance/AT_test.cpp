//
// Created by toebs on 11.04.2020.
//

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, HelloWorld) {
    std::string code = R"(

    def Hello(name : string) : string {
        return "Hello " + name
    }

    var x: string = Hello("World");
    print(x);
    )";
    testing::internal::CaptureStdout();
    context_.interpreter->interpret(code);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("Hello World\n", output);
}

TEST_F(BaseTest, Scoping) {
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

    testing::internal::CaptureStdout();
    context_.interpreter->interpret(code);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ("global\nglobal\n", output);
}