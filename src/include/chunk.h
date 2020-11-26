//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_CHUNK_H_
#define ANGREAL_SRC_CHUNK_H_

#include "common.h"
#include "value.h"

namespace angreal {

enum class OpCode : uint8_t {
    Return,
    Constant,
};

struct Chunk final {
   public:
    using line_t = uint32_t;

    Chunk() = default;
    ~Chunk() = default;

    void WriteChunk(OpCode op_code, uint32_t line);
    void WriteChunk(uint8_t byte, uint32_t line);

    size_t Count() const;
    uint8_t Get(size_t offset) const;
    line_t GetLine(size_t offset) const;

    ValueArray& Constants();

    PRIVATE : std::vector<uint8_t> codes_;
    ValueArray constants_;
    std::vector<line_t> lines_;
};

}  // namespace angreal
#endif  // ANGREAL_SRC_CHUNK_H_
