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

    virtual void disassembleChunk(Chunk* chunk, const char* name);
    virtual size_t disassembleInstruction(Chunk* chunk, size_t offset);
    virtual void TraceStack(Stack<value_t>& stack);

    virtual int unkownOpCode(uint8_t code, size_t offset);
    virtual int simpleInstruction(const std::string& name, size_t offset);
    virtual int constantInstruction(const std::string& name, Chunk* chunk,
                                    size_t offset);
};

}  // namespace angreal
#endif  // ANGREAL_SRC_LIBANGREAL_DEBUG_H_
