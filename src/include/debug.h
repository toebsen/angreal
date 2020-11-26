//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_DEBUG_H_
#define ANGREAL_SRC_LIBANGREAL_DEBUG_H_

namespace angreal {

struct Chunk;

namespace debug {

class Debugger {
   public:
    virtual void disassembleChunk(Chunk* chunk, const char* name) = 0;
    virtual int disassembleInstruction(Chunk* chunk, int offset) = 0;
};

class ConsoleDebugger : public Debugger {
   public:
    void disassembleChunk(Chunk* chunk, const char* name) override;
    int disassembleInstruction(Chunk* chunk, int offset) override;
};

}  // namespace debug

}  // namespace angreal
#endif  // ANGREAL_SRC_LIBANGREAL_DEBUG_H_
