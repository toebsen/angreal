//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_

#include <memory>

namespace tb_lang::interpreter::environment {

class Type {
   public:
    enum class TypeName {
        Bool,
        Int,
        Float,
        String,
    };

    Type(TypeName type_name) : type_name_(type_name) {}

    virtual ~Type() = default;

    TypeName GetTypeName() const { return type_name_; };

   private:
    TypeName type_name_;
};

using type_t = std::shared_ptr<Type>;
}  // namespace tb_lang::interpreter::environment

#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_TYPE_H_
