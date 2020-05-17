//
// Created by toebs on 17.05.2020.
//

#include <string>

#include <gtest/gtest.h>

#include "test_fixtures.h"

TEST_F(ClassTest, DefaultConstructor) {
    DeclareEmptyClass("MyClass");
    auto call = std::make_shared<FunctionCall>(last_class_, kNoArgs);
    context_.interpreter->visit(call);
    ASSERT_EQ(GetResultType()->IsInstance(), true);
    ASSERT_EQ(GetResultType()->AsInstance()->Stringify(),
              "Instance of class(MyClass)");
}
