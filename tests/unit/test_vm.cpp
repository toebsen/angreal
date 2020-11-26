//
// Created by toebsen on 26.11.2020.
//
#include <gtest/gtest.h>

#include "chunk.h"
#include "value.h"
#include "virtual_machine.h"

using namespace angreal;
using namespace testing;

class UtVMTest : public testing::Test {
   protected:
    void SetUp() override { vm.Init(); };
    void TearDown() override { vm.DeInit(); };
    const value_t kArbitraryValue {42};
    VirtualMachine vm;
};

TEST_F(UtVMTest, WhenInitial_ThenNoValuesPresent) {
    ASSERT_EQ(nullptr, vm.ip_);
    ASSERT_EQ(nullptr, vm.chunk_);
}

TEST_F(UtVMTest, WhenInterpretingReturn_ThenResultIsOk) {
    Chunk chunk;

    chunk.WriteChunk(OpCode::Return, 0);

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
}

TEST_F(UtVMTest, WhenInterpretingConstant_ThenResultIsOk) {
    Chunk chunk;
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);

    chunk.WriteChunk(OpCode::Return, 0);
    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
}
