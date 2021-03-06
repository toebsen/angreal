//
// Created by toebs on 11.04.2020.
//
#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ScopingTest, ScopingWithFunction) {
    std::string code = R"(
    var x = "global"
    {
        def printVar() {
            print(x)
            return 0
        }

        printVar()
        var x = "local"
        printVar()
    }
    )";
    SafeRun(code, "\"global\"\n\"global\"");
}

TEST_F(ScopingTest, NestedScopes) {
    std::string code = R"(
    var x = 0
    {
        var x = 1
        {
            var x = 2
            {
                var x = 3
                {
                    var x = 4
                    print(x)
                }
                print(x)
            }
            print(x)
        }
        print(x)
    }
    print(x)
    )";
    SafeRun(code, "4\n3\n2\n1\n0");
}