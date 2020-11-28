//
// Created by toebsen on 26.11.2020.
//
#include "debug.h"

#include <cstdio>

#include "chunk.h"

namespace angreal {

int DebugTracer::simpleInstruction(const std::string& name, size_t offset) {
    printf("%s\n", name.c_str());
    return offset + 1;
}

int DebugTracer::constantInstruction(const std::string& name, Chunk* chunk,
                                     size_t offset) {
    auto constant = chunk->Get(offset + 1);
    printf("%-16s %4d '", name.c_str(), constant);
    PrintValue(chunk->Constants().Get(constant));
    printf("'\n");
    return offset + 2;
}

void DebugTracer::disassembleChunk(Chunk* chunk, const char* name) {
    printf("== %s ==\n", name);
    for (int offset = 0; offset < chunk->Count();) {
        offset = disassembleInstruction(chunk, offset);
    }
}

size_t DebugTracer::disassembleInstruction(Chunk* chunk, size_t offset) {
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
        case AS_BYTE(OpCode::Negate):
            return simpleInstruction("OP_NEGATE", offset);
        case AS_BYTE(OpCode::Add):
            return simpleInstruction("OP_ADD", offset);
        case AS_BYTE(OpCode::Subtract):
            return simpleInstruction("OP_SUBTRACT", offset);
        case AS_BYTE(OpCode::Multiply):
            return simpleInstruction("OP_MULTIPLY", offset);
        case AS_BYTE(OpCode::Divide):
            return simpleInstruction("OP_DIVIDE", offset);
        case AS_BYTE(OpCode::Constant):
            return constantInstruction("OP_CONSTANT", chunk, offset);
        default:
            return unkownOpCode(instruction, offset);
    }
}

void DebugTracer::TraceStack(Stack<value_t>& stack) {
    printf("          ");
    for (auto it = stack.cbegin(); it != stack.cend(); it++) {
        printf("[ ");
        PrintValue((*it));
        printf(" ]");
    }
    printf("\n");
}

int DebugTracer::unkownOpCode(uint8_t code, size_t offset) {
    printf("Unknown opcode %d\n", code);
    return offset + 1;
}
}  // namespace angreal::debug