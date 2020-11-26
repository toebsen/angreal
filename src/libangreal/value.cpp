//
// Created by toebsen on 26.11.2020.
//
#include "value.h"

#include <value.h>

namespace angreal {
size_t ValueArray::Write(const value_t& value) {
    values_.push_back(value);
    return (values_.size() - 1);
}

const value_t& angreal::ValueArray::Peek() const { return values_.back(); }

const value_t& ValueArray::Get(size_t offset) const {
    return values_.at(offset);
}

void PrintValue(const value_t& value) { printf("%g", value); }
}  // namespace angreal
