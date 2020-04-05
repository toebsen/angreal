//
// Created by toebs on 04.04.2020.
//

#include "environment.h"
#include "literal_types.h"

namespace tb_lang::interpreter::environment {

obj_t Environment::Get(const std::string& name) {
    if (auto it = name_to_id_.find(name);
        it != name_to_id_.end() && objects_.contains(it->second)) {
        return objects_[it->second];
    }
    return {};
}

obj_t Environment::Get(const ObjectID& id) {
    if (objects_.contains(id)) {
        return objects_[id];
    }
    return {};
}

void Environment::Remove(obj_t& obj) {
    if (obj) {
        objects_.erase(obj->ID());
        name_to_id_.erase(id_to_name_[obj->ID()]);
    }
}

void Environment::Declare(std::string name, const obj_t& obj) {
    std::string _name = std::move(name);
    if (obj) {
        objects_[obj->ID()] = obj;
        name_to_id_[_name] = obj->ID();
        id_to_name_[obj->ID()] = _name;
    }
}

}  // namespace tb_lang::interpreter::environment