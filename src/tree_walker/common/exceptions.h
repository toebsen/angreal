//
// Created by toebs on 13.04.2020.
//

#ifndef ANGREAL_SRC_COMMON_EXCEPTIONS_H_
#define ANGREAL_SRC_COMMON_EXCEPTIONS_H_
#include <stdexcept>
namespace angreal {

class RuntimeError : public std::runtime_error {
   public:
    explicit RuntimeError(const std::string& msg) : std::runtime_error(msg) {};
};

}  // namespace angreal
#endif  // ANGREAL_SRC_COMMON_EXCEPTIONS_H_
