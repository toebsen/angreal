//
// Created by toebs on 04.04.2020.
//

#include "vm.h"

#include "literal_types.h"

namespace tb_lang::interpreter::virtual_machine {

obj_t VM::Get(const std::string& name) {
    if (auto it = name_to_id_.find(name);
        it != name_to_id_.end() && objects_.contains(it->second)) {
        return objects_[it->second];
    }
    return {};
}

obj_t VM::Get(const ObjectID& id) {
    if (objects_.contains(id)) {
        return objects_[id];
    }
    return {};
}

void VM::Remove(obj_t& obj) {
    if (obj) {
        objects_.erase(obj->ID());
        name_to_id_.erase(id_to_name_[obj->ID()]);
    }
}

void VM::Declare(const std::string& name, const obj_t& obj) {
    if (obj) {
        objects_[obj->ID()] = obj;
        name_to_id_[name] = obj->ID();
        id_to_name_[obj->ID()] = name;
    }
}

void VM::Declare(const std::string& name, int value) {
    obj_t obj = std::make_shared<Object>(std::make_unique<IntType>(value));
    Declare(name, obj);
}

void VM::Declare(const std::string& name, float value) {
    obj_t obj = std::make_shared<Object>(std::make_unique<FloatType>(value));
    Declare(name, obj);
}
void VM::Declare(const std::string& name, bool value) {
    obj_t obj = std::make_shared<Object>(std::make_unique<BoolType>(value));
    Declare(name, obj);
}
void VM::Declare(const std::string& name, const tb_lang::string_t& value) {
    obj_t obj = std::make_shared<Object>(std::make_unique<StringType>(value));
    Declare(name, obj);
}

}  // namespace tb_lang::interpreter::virtual_machine