//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_

#include "common.h"
#include "type.h"

namespace tb_lang::interpreter::virtual_machine {

template <typename T>
class LiteralType : public Type {
   public:
    LiteralType(const T& value);

    virtual T Value() const;
    virtual void setValue(const T& value);

   private:
    T value_;
};

class IntType : public LiteralType<int> {
   public:
    IntType(const int& value);
};

class FloatType : public LiteralType<float> {
   public:
    FloatType(const float& value);
};

class StringType : public LiteralType<tb_lang::string_t> {
   public:
    StringType(const tb_lang::string_t& value);
};

class BoolType : public LiteralType<bool> {
   public:
    BoolType(const bool& value);
};

}  // namespace tb_lang::interpreter::virtual_machine
#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_LITERAL_TYPES_H_
