//
// Created by toebsen on 26.11.2020.
//

#include <gtest/gtest.h>

#include "chunk.h"

using namespace angreal;

class UtChunkTest : public testing::Test {
   protected:
    Chunk chunk;
};

TEST_F(UtChunkTest, WhenInitial_ThenNothingIsPresent) {
    ASSERT_EQ(0, chunk.Count());
    ASSERT_TRUE(chunk.lines_.empty());
    ASSERT_TRUE(chunk.Constants().values_.empty());
}

TEST_F(UtChunkTest, WhenOpCodeIsWritten_ThenOpCodeIsPresentAtOffsetZero) {
    chunk.WriteChunk(OpCode::Return, 123);

    ASSERT_EQ(AS_BYTE(OpCode::Return), chunk.codes_[0]);
    ASSERT_EQ(123U, chunk.lines_[0]);
    ASSERT_EQ(1, chunk.Count());
}

TEST_F(UtChunkTest, WhenByteIsWritten_ThenByteIsPresentAtOffsetZero) {
    chunk.WriteChunk(0xFF, 321);

    ASSERT_EQ(AS_BYTE(0xFF), chunk.codes_[0]);
    ASSERT_EQ(321U, chunk.lines_[0]);
    ASSERT_EQ(1, chunk.Count());
}

TEST_F(UtChunkTest, WhenOpCodeIsWrittenTwice_ThenOpCodesArePresentAtOffsets) {
    chunk.WriteChunk(OpCode::Constant, 123);
    chunk.WriteChunk(OpCode::Return, 321);

    ASSERT_EQ(AS_BYTE(OpCode::Return), chunk.Get(1));
    ASSERT_EQ(321, chunk.GetLine(1));
    ASSERT_EQ(2, chunk.Count());
}