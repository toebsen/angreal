//
// Created by toebs on 04.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_
#define TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_

#include <cstddef>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "common.h"
#include "type.h"

namespace tb_lang::interpreter::environment {

using ObjectID = boost::uuids::uuid;

class Type;
using type_t = std::shared_ptr<Type>;

class Object final: public NonCopyable{
   public:
    explicit Object();
    explicit Object(type_t type);

    ~Object() override = default;

    bool operator==(const Object& rhs) const;

    [[nodiscard]] bool HasSameType(const Object& rhs) const;

    [[nodiscard]] ObjectID ID() const;
    [[nodiscard]] const type_t& GetType() const;

   private:
    ObjectID id_;
    type_t type_;
};

using obj_t = std::shared_ptr<Object>;

}  // namespace tb_lang::interpreter::environment
#endif  // TBLANG_SRC_INTERPRETER_VIRTUAL_MACHINE_OBJECT_H_