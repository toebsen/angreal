//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_

#include <any>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "common.h"
#include "type.h"

namespace tb_lang::interpreter::environment {

using ObjectID = boost::uuids::uuid;

class Object {
   public:
    explicit Object(type_t type);

    Object(const Object& object) = delete;
    Object(Object&& object) noexcept = delete;
    virtual ~Object() = default;

    Object& operator=(const Object& object) = delete;
    Object& operator=(Object&& object) = delete;

    bool operator==(const Object& rhs) const;

    [[nodiscard]] ObjectID ID() const;
    [[nodiscard]] type_t& GetType();

   private:
    ObjectID id_;
    type_t type_;
};

}  // namespace tb_lang::interpreter::environment
#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_