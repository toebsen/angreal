//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_

#include <any>
#include <map>
#include <memory>

#include "literal_types.h"
#include "object.h"

namespace tb_lang::interpreter::virtual_machine {
using obj_t = std::shared_ptr<Object>;

class VM {
   public:
    VM() = default;

    virtual ~VM() = default;

    VM(const VM& object) = delete;
    VM(VM&& object) noexcept = delete;

    VM& operator=(const VM& vm) = delete;
    VM& operator=(VM&& vm) = delete;

    obj_t Get(const std::string& name);
    obj_t Get(const ObjectID &id);

    void Declare(const std::string& name, const obj_t& obj);
    void Declare(const std::string& name, int value);
    void Declare(const std::string& name, bool value);
    void Declare(const std::string& name, float value);

    void Declare(const std::string& name, const tb_lang::string_t& value);

   private:
    std::map<ObjectID, obj_t> objects_;

    std::map<ObjectID, tb_lang::string_t> id_to_name_;
    std::map<tb_lang::string_t, ObjectID> name_to_id_;
    void Remove(obj_t& obj);
};

}  // namespace tb_lang::interpreter::virtual_machine

#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_
