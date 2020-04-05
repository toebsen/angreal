//
// Created by toebs on 04.04.2020.
//
#include "object.h"

namespace tb_lang::interpreter::environment {

namespace ID {
static ObjectID Create() { return boost::uuids::random_generator()(); }
}  // namespace ID

Object::Object(type_t type) : id_(ID::Create()), type_(std::move(type)) {}

bool Object::operator==(const Object& rhs) const { return ID() == rhs.ID(); }

ObjectID Object::ID() const { return id_; }

type_t& Object::GetType() { return type_; }

}  // namespace tb_lang::interpreter::environment