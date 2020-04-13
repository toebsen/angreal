//
// Created by toebs on 13.04.2020.
//

#ifndef TBLANG_SRC_COMMON_EXCEPTIONS_H_
#define TBLANG_SRC_COMMON_EXCEPTIONS_H_
#include <stdexcept>
namespace tb_lang {

class RuntimeError : public std::runtime_error {
   public:
    explicit RuntimeError(const std::string& msg) : std::runtime_error(msg){};
};

}  // namespace tb_lang
#endif  // TBLANG_SRC_COMMON_EXCEPTIONS_H_
