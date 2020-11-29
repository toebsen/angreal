//
// Created by toebsen on 28.11.2020.
//
#ifndef ANGREAL_TESTS_MOCKS_MOCK_DEBUG_TRACER_H_
#define ANGREAL_TESTS_MOCKS_MOCK_DEBUG_TRACER_H_

#include <gmock/gmock.h>

#include "debug.h"
#include "virtual_machine.h"

namespace angreal {
namespace mocks {
class MockDebugTracer : public DebugTracer {
 public:
  MOCK_METHOD(void, TraceStack, (VirtualMachine::ValueStack & ));

  MOCK_METHOD(int, SimpleInstruction, (const std::string&, size_t));
  MOCK_METHOD(int, ConstantInstruction, (const std::string&, Chunk*, size_t));
  MOCK_METHOD(int, UnknownOpCode, (uint8_t, size_t));
  MOCK_METHOD(void, PrintPreamble, (Chunk * , size_t));
};
}  // namespace mocks
}  // namespace angreal
#endif  // ANGREAL_TESTS_MOCKS_MOCK_DEBUG_TRACER_H_