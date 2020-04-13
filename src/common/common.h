//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_COMMON_COMMON_H_
#define TBLANG_SRC_COMMON_COMMON_H_

#include <string>
#include "exceptions.h"

namespace tb_lang {
using string_t = std::string;

class NonCopyable {
   public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;

   protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
};

class Copyable {
   public:
    Copyable(const Copyable&) = default;
    Copyable(Copyable&&) = delete;
    Copyable& operator=(const Copyable&) = default;
    Copyable& operator=(Copyable&&) = delete;

   protected:
    Copyable() = default;
    virtual ~Copyable() = default;
};

class Movable {
   public:
    Movable(const Movable&) = delete;
    Movable(Movable&&) = default;
    Movable& operator=(const Movable&) = delete;
    Movable& operator=(Movable&&) = default;

   protected:
    Movable() = default;
    virtual ~Movable() = default;
};

}  // namespace tb_lang

#endif  // TBLANG_SRC_COMMON_COMMON_H_
