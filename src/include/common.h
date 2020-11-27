//
// Created by toebsen on 26.11.2020.
//

#ifndef ANGREAL_SRC_COMMON_H_
#define ANGREAL_SRC_COMMON_H_

#include <deque>
#include <memory>
#include <ostream>
#include <stack>
#include <vector>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef ENABLE_TESTS
#define PRIVATE public
#else
#define PRIVATE private
#endif

#define AS_BYTE(value) static_cast<uint8_t>(value)

namespace angreal {}  // namespace angreal
#endif                // ANGREAL_SRC_COMMON_H_
