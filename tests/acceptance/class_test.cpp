//
// Created by toebs on 17.05.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(BaseTest, EmptyClass) {
    std::string code = R"(
        class MyClass
        {
        }
        print(MyClass)

        var myclass = MyClass()
        print(myclass)
    )";

    SafeRun(code, "class(MyClass)\nInstance of class(MyClass)");
}