//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_

#include <memory>

namespace tb_lang::interpreter::virtual_machine {

class Type {
   public:
    virtual ~Type() = default;

    //   public:
    //    enum class Type
    //    {
    //        Bool,
    //        Int,
    //        Float,
    //        String,
    //    };
};

using type_t = std::shared_ptr<Type>;
}  // namespace tb_lang::interpreter::virtual_machine

#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
