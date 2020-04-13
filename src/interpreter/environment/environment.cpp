//
// Created by toebs on 04.04.2020.
//

#include "environment.h"

#include "literal_types.h"

namespace tb_lang::interpreter::environment {

Environment::Environment(const environment_t& enclosing)
    : enclosing_(enclosing) {}

obj_t Environment::Get(const std::string& name) {
    if (auto it = name_to_id_.find(name); it != name_to_id_.end()) {
        return Get(it->second);
    }

    if (enclosing_) {
        return enclosing_->Get(name);
    }
    throw RuntimeError("<" + name + "> is not defined!");
}

obj_t Environment::Get(const ObjectID& id) {
    if (objects_.contains(id)) {
        return objects_[id];
    }
    return {};
}

void Environment::Remove(obj_t& obj) {
    if (obj && Contains(obj)) {
        objects_.erase(obj->ID());
        name_to_id_.erase(id_to_name_[obj->ID()]);
        id_to_name_.erase(obj->ID());
    }
}

void Environment::Remove(const std::string& name) {
    if (!name.empty() && Contains(name)) {
        auto obj_id = name_to_id_[name];
        objects_.erase(obj_id);
        name_to_id_.erase(id_to_name_[obj_id]);
        id_to_name_.erase(obj_id);
    }
}

void Environment::Declare(const std::string& name, const obj_t& obj) {
    if (obj) {
        if (!Contains(obj)) {
            objects_[obj->ID()] = obj;
            name_to_id_[name] = obj->ID();
            id_to_name_[obj->ID()] = name;
        } else {
            // Todo: Redeclaration - intentional? -> Warning?
        }
    } else {
        throw RuntimeError("Cannot declare an empty with name <" + name + ">!");
    }
}

void Environment::Assign(const std::string& name, const obj_t& obj) {
    if (obj) {
        if (Contains(name)) {
            Remove(name);
            objects_[obj->ID()] = obj;
            name_to_id_[name] = obj->ID();
            id_to_name_[obj->ID()] = name;
        } else {
            if (enclosing_) {
                enclosing_->Assign(name, obj);
                return;
            }
        }
    } else {
        throw RuntimeError("Cannot assign empty value to name <" + name + ">!");
    }
}

bool Environment::Contains(const obj_t& obj) const {
    return objects_.find(obj->ID()) != objects_.end();
}
bool Environment::Contains(const std::string& name) const {
    return name_to_id_.find(name) != name_to_id_.end();
}

}  // namespace tb_lang::interpreter::environment