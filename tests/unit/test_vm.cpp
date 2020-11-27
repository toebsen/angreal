//
// Created by toebsen on 26.11.2020.
//
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#define DEBUG_TRACE_EXECUTION

#include "chunk.h"
#include "value.h"
#include "virtual_machine.h"

using namespace angreal;

using testing::ElementsAre;

class FakeStackTracer : public StackTracer {
   public:
    void TraceStack(const VirtualMachine::ValueStack& stack) {
        for (auto it = stack.cbegin(); it != stack.cend(); it++) {
            history.push_back(*it);
        }
        StackTracer::TraceStack(stack);
    }

    std::vector<value_t> history;
};

class UtVMTest : public testing::Test {
   protected:
    void SetUp() override { vm.Init(); };

    void TearDown() override {
        ASSERT_TRUE(vm.value_stack_.empty());
        vm.DeInit();
    };

    const value_t kArbitraryValue {42};
    Chunk chunk;
    FakeStackTracer tracer;
    VirtualMachine vm {tracer};
};

TEST_F(UtVMTest, WhenInitial_ThenNoValuesPresent) {
    ASSERT_EQ(nullptr, vm.ip_);
    ASSERT_EQ(nullptr, vm.chunk_);
}

TEST_F(UtVMTest, WhenInterpretingReturn_ThenResultIsOk) {
    chunk.WriteChunk(OpCode::Return, 0);

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
}

TEST_F(UtVMTest, WhenInterpretingConstant_ThenResultIsOk) {
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);
    chunk.WriteChunk(OpCode::Return, 0);

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(tracer.history, ElementsAre(kArbitraryValue));
}

TEST_F(UtVMTest, WhenInterpretingNegation_ThenResultIsOk) {
    auto address = chunk.Constants().Write(kArbitraryValue);
    chunk.WriteChunk(OpCode::Constant, 0);
    chunk.WriteChunk(address, 0);
    chunk.WriteChunk(OpCode::Negate, 0);
    chunk.WriteChunk(OpCode::Return, 0);

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(tracer.history, ElementsAre(kArbitraryValue, -kArbitraryValue));
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

    ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
    ASSERT_THAT(tracer.history,
                ElementsAre(kArbitraryValue, kArbitraryValue, kArbitraryValue,
                            kArbitraryValue + kArbitraryValue));
}