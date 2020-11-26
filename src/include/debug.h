//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_DEBUG_H_
#define ANGREAL_SRC_LIBANGREAL_DEBUG_H_

#include <cstddef>

namespace angreal {

struct Chunk;

namespace debug {
void disassembleChunk(Chunk* chunk, const char* name);
size_t disassembleInstruction(Chunk* chunk, size_t offset);
}  // namespace debug

}  // namespace angreal
#endif  // ANGREAL_SRC_LIBANGREAL_DEBUG_H_
