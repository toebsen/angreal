//
// Created by toebsen on 26.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_VALUE_ARRAY_H_
#define ANGREAL_SRC_LIBANGREAL_VALUE_ARRAY_H_

#include "common.h"

namespace angreal {
using value_t = double;

class ValueArray final {
   public:
    ValueArray() = default;

    size_t Write(const value_t& value);
    const value_t& Get(size_t offset) const;
    const value_t& Peek() const;

   private:
    std::vector<value_t> values_;
};

void PrintValue(const value_t& value);
}  // namespace angreal

#endif  // ANGREAL_SRC_LIBANGREAL_VALUE_ARRAY_H_
