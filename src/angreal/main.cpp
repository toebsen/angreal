//
// Created by toebsen on 26.11.2020.
//

#include "chunk.h"
#include "common.h"
#include "debug.h"

using namespace angreal;

int main(int argc, const char* argv[]) {
    Chunk chunk;
    debug::ConsoleDebugger console_debugger;
    auto constant = chunk.Constants().Write(1.2f);
    chunk.WriteChunk(OpCode::Constant, 123);
    chunk.WriteChunk(constant, 123);

    chunk.WriteChunk(OpCode::Return, 124);
    console_debugger.disassembleChunk(&chunk, "test");
    return 0;
}