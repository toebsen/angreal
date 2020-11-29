//
// Created by toebsen on 29.11.2020.
//
#ifndef ANGREAL_TESTS_UNIT_UT_TEST_VM_H_
#define ANGREAL_TESTS_UNIT_UT_TEST_VM_H_

#include <gmock/gmock.h>

#include "chunk.h"
#include "value.h"
#include "virtual_machine.h"

#include "mock_debug_tracer.h"
using ::testing::NiceMock;

namespace angreal {
class UtVMTest : public ::testing::Test {

 public:
  static constexpr value_t kArbitraryValue{42};

 protected:
  void SetUp() override;
  void TearDown() override;

  void SetMockActions();
  void AddStackToHistory(const VirtualMachine::ValueStack &vs);

 protected:

  NiceMock<mocks::MockDebugTracer> mock_debug_tracer;
  Chunk chunk;
  VirtualMachine vm{mock_debug_tracer};
  std::vector<value_t> stack_history;
};
}

#endif //ANGREAL_TESTS_UNIT_UT_TEST_VM_H_
