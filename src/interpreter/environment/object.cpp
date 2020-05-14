//
// Created by toebs on 04.04.2020.
//
#include "object.h"

namespace angreal::interpreter::environment {

namespace ID {
static ObjectID Create() { return boost::uuids::random_generator()(); }
}  // namespace ID
Object::Object() : id_(ID::Create()), type_(nullptr) {}

Object::Object(type_t type) : id_(ID::Create()), type_(std::move(type)) {}

bool Object::operator==(const Object& rhs) const { return ID() == rhs.ID(); }

ObjectID Object::ID() const { return id_; }

const type_t& Object::GetType() const { return type_; }

bool Object::HasSameType(const Object& rhs) const {
    if (!GetType() || !rhs.GetType()) {
        return false;
    }
    return GetType()->HasSameType(*rhs.GetType());
}

}  // namespace angreal::interpreter::environment