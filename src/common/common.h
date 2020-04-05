//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_COMMON_COMMON_H_
#define TBLANG_SRC_COMMON_COMMON_H_

#include <string>

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
}  // namespace tb_lang

#endif  // TBLANG_SRC_COMMON_COMMON_H_
