//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_

#include <any>
#include <iostream>
#include <map>
#include <memory>

#include "literal_types.h"
#include "object.h"

namespace tb_lang::interpreter::environment {


using obj_t = std::shared_ptr<Object>;

class Environment {
   public:
    Environment() = default;

    virtual ~Environment() = default;

    Environment(const Environment& object) = delete;
    Environment(Environment&& object) noexcept = delete;

    Environment& operator=(const Environment& vm) = delete;
    Environment& operator=(Environment&& vm) = delete;

    obj_t Get(const std::string& name);
    obj_t Get(const ObjectID& id);

    void Declare(std::string name, const obj_t& obj);

   private:
    std::map<ObjectID, obj_t> objects_;

    std::map<ObjectID, tb_lang::string_t> id_to_name_;
    std::map<tb_lang::string_t, ObjectID> name_to_id_;
    void Remove(obj_t& obj);
};

}  // namespace tb_lang::interpreter::environment

#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_
