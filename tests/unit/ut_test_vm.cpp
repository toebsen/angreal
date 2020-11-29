//
// Created by toebsen on 29.11.2020.
//
#include "ut_test_vm.h"

namespace angreal {

using ::testing::Invoke;
using ::testing::WithArg;

namespace {
constexpr auto kOffsetPlusOne = [](size_t offset) -> int { return offset + 1; };
constexpr auto kOffsetPlusTwo = [](size_t offset) -> int { return offset + 2; };
}

void UtVMTest::SetUp() {
  SetMockActions();
  vm.Init();
};

void UtVMTest::TearDown() {
  ASSERT_TRUE(vm.value_stack_.empty());
  vm.DeInit();
};

void UtVMTest::SetMockActions() {
  ON_CALL(mock_debug_tracer, TraceStack)
      .WillByDefault(Invoke(this, &UtVMTest::AddStackToHistory));

  ON_CALL(mock_debug_tracer, SimpleInstruction)
      .WillByDefault(WithArg<1>(Invoke(kOffsetPlusOne)));

  ON_CALL(mock_debug_tracer, UnknownOpCode)
      .WillByDefault(WithArg<1>(Invoke(kOffsetPlusOne)));

  ON_CALL(mock_debug_tracer, ConstantInstruction)
      .WillByDefault(WithArg<2>(Invoke(kOffsetPlusTwo)));
}

void UtVMTest::AddStackToHistory(const VirtualMachine::ValueStack &vs) {
  for (auto it = vs.cbegin(); it != vs.cend(); ++it) {
    stack_history.push_back((*it));
  }
}
}