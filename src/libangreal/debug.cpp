//
// Created by toebsen on 26.11.2020.
//
#include "debug.h"

#include <cstdio>

#include <magic_enum.hpp>

#include "chunk.h"

namespace angreal {

int DebugTracer::SimpleInstruction(const std::string &name, size_t offset) {
  printf("%s\n", name.c_str());
  return offset + 1;
}

int DebugTracer::ConstantInstruction(const std::string &name, Chunk *chunk,
                                     size_t offset) {
  auto constant = chunk->Get(offset + 1);
  printf("%-16s %4d '", name.c_str(), constant);
  PrintValue(chunk->Constants().Get(constant));
  printf("'\n");
  return offset + 2;
}

void DebugTracer::DisassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);
  for (int offset = 0; offset < chunk->Count();) {
    offset = DisassembleInstruction(chunk, offset);
  }
}

size_t DebugTracer::DisassembleInstruction(Chunk *chunk, size_t offset) {
  PrintPreamble(chunk, offset);

  auto instruction = chunk->Get(offset);
  std::string instruction_name{
      magic_enum::enum_name(static_cast<OpCode>(instruction))};
  instruction_name = "Op_" + instruction_name;
  switch (instruction) {
    case AS_BYTE(OpCode::Return):return SimpleInstruction(instruction_name, offset);
    case AS_BYTE(OpCode::Negate):return SimpleInstruction(instruction_name, offset);
    case AS_BYTE(OpCode::Add):return SimpleInstruction(instruction_name, offset);
    case AS_BYTE(OpCode::Subtract):return SimpleInstruction(instruction_name, offset);
    case AS_BYTE(OpCode::Multiply):return SimpleInstruction(instruction_name, offset);
        case AS_BYTE(OpCode::Divide):return SimpleInstruction(instruction_name, offset);
        case AS_BYTE(OpCode::Constant):return ConstantInstruction(instruction_name, chunk, offset);
        default:return UnknownOpCode(instruction, offset);
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

int DebugTracer::UnknownOpCode(uint8_t code, size_t offset) {
  printf("Unknown opcode %d\n", code);
  return offset + 1;
}
void DebugTracer::PrintPreamble(Chunk *chunk, size_t offset) {
  printf("%04d ", offset);
  if (offset > 0 && chunk->GetLine(offset) == chunk->GetLine(offset - 1)) {
    printf("   | ");
  } else {
    printf("%4d ", chunk->GetLine(offset));
  }
}
}  // namespace angreal::debug