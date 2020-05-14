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

class Environment final : public NonCopyable,
                          public std::enable_shared_from_this<Environment> {
   public:
    Environment() = default;
    explicit Environment(const environment_t& enclosing);

    obj_t Get(const std::string& name);
    obj_t Get(const std::string& name, size_t distance);

    void Declare(const std::string& name, const obj_t& obj);
    void Declare(const std::string& name, const obj_t& obj, size_t distance);

    void Assign(const std::string& name, const obj_t& obj);
    void Assign(const std::string& name, const obj_t& obj, size_t distance);

   private:
    [[nodiscard]] inline obj_t Get(const ObjectID& id);

    [[nodiscard]] inline bool Contains(const obj_t& obj) const;
    [[nodiscard]] inline bool Contains(const std::string& name) const;

    environment_t Ancestor(size_t distance);

    void Remove(obj_t& obj);
    void Remove(const std::string& name);

    environment_t enclosing_;

    std::map<ObjectID, obj_t> objects_;
    std::map<ObjectID, tb_lang::string_t> id_to_name_;
    std::map<tb_lang::string_t, ObjectID> name_to_id_;
};

}  // namespace tb_lang::interpreter::environment

#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_VM_H_
