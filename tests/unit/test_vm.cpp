//
// Created by toebsen on 26.11.2020.
//

#define DEBUG_TRACE_EXECUTION

#include <vector>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "chunk.h"
#include "mock_debug_tracer.h"
#include "value.h"
#include "virtual_machine.h"

using namespace angreal;
using namespace ::testing;

using testing::_;
using testing::ElementsAre;
using testing::Invoke;
using testing::NiceMock;
using testing::WithArg;

constexpr auto kOffsetPlusOne = [](size_t offset) -> int { return offset + 1; };
constexpr auto kOffsetPlusTwo = [](size_t offset) -> int { return offset + 2; };

class UtVMTest : public testing::Test {
   protected:
    void SetUp() override {
        // setup mock for stack history
        ON_CALL(mock_debug_tracer, TraceStack)
            .WillByDefault(Invoke(this, &UtVMTest::AddStackToHistory));

        ON_CALL(mock_debug_tracer, simpleInstruction)
            .WillByDefault(WithArg<1>(Invoke(kOffsetPlusOne)));
        ON_CALL(mock_debug_tracer, unkownOpCode)
            .WillByDefault(WithArg<1>(Invoke(kOffsetPlusOne)));

        ON_CALL(mock_debug_tracer, constantInstruction)
            .WillByDefault(WithArg<2>(Invoke(kOffsetPlusTwo)));
        vm.Init();
    };

    void TearDown() override {
        ASSERT_TRUE(vm.value_stack_.empty());
        vm.DeInit();
    };

    void AddStackToHistory(const VirtualMachine::ValueStack& vs) {
        for (auto it = vs.cbegin(); it != vs.cend(); ++it) {
            stack_history.push_back((*it));
        }
    }

    const value_t kArbitraryValue {42};
    NiceMock<mocks::MockDebugTracer> mock_debug_tracer;
    Chunk chunk;
    VirtualMachine vm {mock_debug_tracer};
    std::vector<value_t> stack_history;
};

TEST_F(UtVMTest, WhenInitial_ThenNoValuesPresent) {
    ASSERT_EQ(nullptr, vm.ip_);
    ASSERT_EQ(nullptr, vm.chunk_);
}

TEST_F(UtVMTest, WhenInterpretingReturn_ThenResultIsOk) {
    chunk.WriteChunk(OpCode::Return, 0);
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_RETURN", 0));
    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
}

TEST_F(UtVMTest, WhenInterpretingConstant_ThenResultIsOk) {
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);
    chunk.WriteChunk(OpCode::Return, 0);

    EXPECT_CALL(mock_debug_tracer, constantInstruction("OP_CONSTANT", _, 0));
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_RETURN", 2));

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(stack_history, ElementsAre(kArbitraryValue));
}

TEST_F(UtVMTest, WhenInterpretingNegation_ThenResultIsOk) {
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);
    chunk.WriteChunk(OpCode::Negate, 0);
    chunk.WriteChunk(OpCode::Return, 0);

    EXPECT_CALL(mock_debug_tracer, constantInstruction("OP_CONSTANT", _, 0));
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_NEGATE", 2));
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_RETURN", 3));

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(stack_history, ElementsAre(kArbitraryValue, -kArbitraryValue));
}

TEST_F(UtVMTest, WhenInterpretingAddition_ThenResultIsOk) {
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);
    auto address2 = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address2, 0);
    chunk.WriteChunk(OpCode::Add, 0);
    chunk.WriteChunk(OpCode::Return, 0);

    EXPECT_CALL(mock_debug_tracer, constantInstruction("OP_CONSTANT", _, 0));
    EXPECT_CALL(mock_debug_tracer, constantInstruction("OP_CONSTANT", _, 2));
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_ADD", 4));
    EXPECT_CALL(mock_debug_tracer, simpleInstruction("OP_RETURN", 5));

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(stack_history,
                ElementsAre(kArbitraryValue, kArbitraryValue, kArbitraryValue,
                            kArbitraryValue + kArbitraryValue));
}