//
// Created by toebsen on 26.11.2020.
//
#include <gtest/gtest.h>

#include "chunk.h"

using namespace angreal;

class UtValueTest : public testing::Test {
   protected:
    const value_t kArbitraryValue {42};
    ValueArray value_array;
};

TEST_F(UtValueTest, WhenInitial_ThenNoValuesPresent) {
    ASSERT_TRUE(value_array.values_.empty());
}

TEST_F(UtValueTest, WhenValueIsWritten_ThenValueIsPresent) {
    ASSERT_EQ(0U, value_array.Write(kArbitraryValue));

    ASSERT_EQ(kArbitraryValue, value_array.values_[0]);
    ASSERT_EQ(kArbitraryValue, value_array.Peek());
}

TEST_F(UtValueTest, WhenTwoValueAreWritten_ThenValuesArePresentAtOffset) {
    ASSERT_EQ(0U, value_array.Write(kArbitraryValue + 1));

    ASSERT_EQ(kArbitraryValue + 1, value_array.values_[0]);
    ASSERT_EQ(kArbitraryValue + 1, value_array.Get(0));
    ASSERT_EQ(kArbitraryValue + 1, value_array.Peek());

    ASSERT_EQ(1U, value_array.Write(kArbitraryValue - 1));

    ASSERT_EQ(kArbitraryValue - 1, value_array.values_[1]);
    ASSERT_EQ(kArbitraryValue - 1, value_array.Get(1));
    ASSERT_EQ(kArbitraryValue - 1, value_array.Peek());
}