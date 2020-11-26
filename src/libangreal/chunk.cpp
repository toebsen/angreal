//
// Created by toebsen on 26.11.2020.
//

#include "chunk.h"

namespace angreal {
void angreal::Chunk::WriteChunk(OpCode op_code, uint32_t line) {
    WriteChunk(static_cast<uint8_t>(op_code), line);
}

void angreal::Chunk::WriteChunk(uint8_t byte, uint32_t line) {
    codes_.push_back(byte);
    lines_.push_back(line);
}

size_t Chunk::Count() const { return codes_.size(); }
uint8_t Chunk::Get(size_t offset) const { return codes_.at(offset); }

ValueArray& Chunk::Constants() { return constants_; }

Chunk::line_t Chunk::GetLine(size_t offset) const { return lines_.at(offset); }

uint8_t* Chunk::Begin() { return &codes_.front(); }

}  // namespace angreal
