//
// Created by toebsen on 26.11.2020.
//
#include "debug.h"

#include <cstdio>

#include "chunk.h"


namespace angreal::debug {

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    auto constant = chunk->Get(offset + 1);
    printf("%-16s %4d '", name, constant);
    PrintValue(chunk->Constants().Get(constant));
    printf("'\n");
    return offset + 2;
}

void ConsoleDebugger::disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->Count();) {
        offset = disassembleInstruction(chunk, offset);
    }
}

int ConsoleDebugger::disassembleInstruction(Chunk* chunk, int offset) {
    printf("%04d ", offset);
    if (offset > 0 && chunk->GetLine(offset) == chunk->GetLine(offset - 1)) {
        printf("   | ");
    } else {
        printf("%4d ", chunk->GetLine(offset));
    }

    auto instruction = chunk->Get(offset);
    switch (instruction) {
        case AS_BYTE(OpCode::Return):
            return simpleInstruction("OP_RETURN", offset);
        case AS_BYTE(OpCode::Constant):
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

}  // namespace angreal::debug