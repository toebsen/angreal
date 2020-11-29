//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_DEBUG_H_
#define ANGREAL_SRC_LIBANGREAL_DEBUG_H_

#include <cstddef>
#include <string>

#include "stack.h"
#include "value.h"

namespace angreal {

struct Chunk;

class DebugTracer {
 public:
  virtual ~DebugTracer() = default;

  virtual void DisassembleChunk(Chunk *chunk, const char *name);
  virtual size_t DisassembleInstruction(Chunk *chunk, size_t offset);
  virtual void TraceStack(Stack<value_t> &stack);
  virtual void PrintPreamble(Chunk *chunk, size_t offset);
  virtual int UnknownOpCode(uint8_t code, size_t offset);
  virtual int SimpleInstruction(const std::string &name, size_t offset);
  virtual int ConstantInstruction(const std::string &name, Chunk *chunk,
                                  size_t offset);
};

}  // namespace angreal
#endif  // ANGREAL_SRC_LIBANGREAL_DEBUG_H_
