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

TEST_F(BaseTest, ClassGetterSetter) {
    std::string code = R"(
        class MyClass
        {
        }
        var myclass = MyClass()
        myclass.x = 123
        print(myclass.x);
    )";

    SafeRun(code, "123");
}

TEST_F(BaseTest, ClassMethods) {
    std::string code = R"(
    class Bacon
    {
        def eat()
        {
            print("Crunch crunch crunch!")
        }
    }

    Bacon().eat()
    )";

    SafeRun(code, R"("Crunch crunch crunch!")");
}