//
// Created by toebsen on 26.11.2020.
//

#define DEBUG_TRACE_EXECUTION

#include <vector>
#include <string>

#include <gtest/gtest.h>

#include "ut_test_vm.h"

#include "test_with_param.h"

using namespace angreal;

using ::testing::_;
using ::testing::ElementsAre;

TEST_F(UtVMTest, WhenInitial_ThenNoValuesPresent) {
  ASSERT_EQ(nullptr, vm.ip_);
  ASSERT_EQ(nullptr, vm.chunk_);
}

TEST_F(UtVMTest, WhenInterpretingReturn_ThenResultIsOk) {
  chunk.WriteChunk(OpCode::Return, 0);
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction("Op_Return", 0));
  ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
}

TEST_F(UtVMTest, WhenInterpretingConstant_ThenResultIsOk) {
  auto address = chunk.Constants().Write(kArbitraryValue);
  chunk.WriteChunk(OpCode::Constant, 0);
  chunk.WriteChunk(address, 0);
  chunk.WriteChunk(OpCode::Return, 0);

  EXPECT_CALL(mock_debug_tracer, ConstantInstruction("Op_Constant", _, 0));
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction("Op_Return", 2));

  ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
  ASSERT_THAT(stack_history, ElementsAre(kArbitraryValue));
}

TEST_F(UtVMTest, WhenInterpretingBinaryOp_ThenResultIsOk) {
  auto address = chunk.Constants().Write(kArbitraryValue);
  chunk.WriteChunk(OpCode::Constant, 0);
  chunk.WriteChunk(address, 0);
  chunk.WriteChunk(OpCode::Negate, 0);
  chunk.WriteChunk(OpCode::Return, 0);

  EXPECT_CALL(mock_debug_tracer, ConstantInstruction("Op_Constant", _, 0));
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction("Op_Negate", 2));
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction("Op_Return", 3));

  ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
  ASSERT_THAT(stack_history, ElementsAre(kArbitraryValue, -kArbitraryValue));
}

namespace {
struct BinaryOpTestConditions {
  OpCode binary_op;

  std::string expected_binary_op_name;
  value_t expected_result;
};

const BinaryOpTestConditions kBinaryOpTestConditions[] =
    {
        {OpCode::Add, "Op_Add", 2 * UtVMTest::kArbitraryValue},
        {OpCode::Subtract, "Op_Subtract", 0},
        {OpCode::Divide, "Op_Divide", 1},
        {OpCode::Multiply, "Op_Multiply", UtVMTest::kArbitraryValue * UtVMTest::kArbitraryValue},
    };
}

using UtVMTest_P = TestWithParam<UtVMTest, BinaryOpTestConditions>;

INSTANTIATE_TEST_SUITE_P(WithAllBinaryOps,
                         UtVMTest_P,
                         testing::ValuesIn(kBinaryOpTestConditions));

TEST_P(UtVMTest_P, WhenInterpretingAddition_ThenResultIsOk) {
  auto address = chunk.Constants().Write(kArbitraryValue);
  chunk.WriteChunk(OpCode::Constant, 0);
  chunk.WriteChunk(address, 0);
  auto address2 = chunk.Constants().Write(kArbitraryValue);
  chunk.WriteChunk(OpCode::Constant, 0);
  chunk.WriteChunk(address2, 0);
  chunk.WriteChunk(test_vector_.binary_op, 0);
  chunk.WriteChunk(OpCode::Return, 0);

  EXPECT_CALL(mock_debug_tracer, ConstantInstruction("Op_Constant", _, 0));
  EXPECT_CALL(mock_debug_tracer, ConstantInstruction("Op_Constant", _, 2));
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction(test_vector_.expected_binary_op_name, 4));
  EXPECT_CALL(mock_debug_tracer, SimpleInstruction("Op_Return", 5));

  ASSERT_EQ(InterpretResult::Ok, vm.Interpret(&chunk));
  ASSERT_THAT(stack_history,
              ElementsAre(kArbitraryValue, kArbitraryValue, kArbitraryValue,
                          test_vector_.expected_result));
}