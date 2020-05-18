//
// Created by toebs on 11.04.2020.
//

#include "callable.h"

#include "../interpreter.h"

namespace angreal::interpreter::environment {

Function::Function(std::shared_ptr<FunctionDeclaration> function_decl,
                   environment_t env)
    : function_decl_(std::move(function_decl)), env_(std::move(env)) {}

bool Function::CheckArity(const std::vector<obj_t>& args) const {
    return args.size() == function_decl_->parameters.size();
}

size_t Function::Arity() const { return function_decl_->parameters.size(); }

obj_t Function::Call(const interpreter_t& interp,
                     const std::vector<obj_t>& args) {
    auto local_env = std::make_shared<Environment>(env_);

    if (!CheckArity(args)) {
        std::stringstream ss;
        ss << "Called function <" << function_decl_->identifier
           << "> with wrong args";
        ss << "Expected <" << Arity() << "> but got: " << args.size()
           << std::endl;
        throw RuntimeError(ss.str());
    }

    for (size_t kI = 0; kI < args.size(); ++kI) {
        auto param_decl = function_decl_->parameters[kI];
        local_env->Declare(function_decl_->parameters[kI]->identifier,
                           args[kI]);
    }

    try {
        interp->invoke(function_decl_->statements, local_env);
    } catch (obj_t& ret) {
        return ret;
    }

    return obj_t();
}

string_t Function::Stringify() const {
    return "function(" + function_decl_->identifier + ")";
}

Class::Class(std::shared_ptr<ClassDeclaration> class_declaration,
             environment_t env)
    : class_declaration_(std::move(class_declaration)), env_(std::move(env)) {}

bool Class::CheckArity(const std::vector<obj_t>& args) const { return true; }

size_t Class::Arity() const { return 0; }

obj_t Class::Call(interpreter_t const& interp, const std::vector<obj_t>& args) {
    instance_t instance = std::make_shared<Instance>(shared_from_this());
    auto type = std::make_shared<Type>(instance);
    return std::make_shared<Object>(type);
}

string_t Class::Stringify() const {
    return "class(" + class_declaration_->identifier + ")";
}

Instance::Instance(std::shared_ptr<Class> _class) : class_(std::move(_class)) {}

string_t Instance::Stringify() const {
    return "Instance of " + class_->Stringify();
}

obj_t Instance::Get(const string_t& name) {
    if (fields_.contains(name)) {
        return fields_[name];
    }

    throw RuntimeError("Undefined property <" + name + "> of " +
                       class_->Stringify());
}
void Instance::Set(const string_t& name, const obj_t& value) {
    fields_[name] = value;
}

}  // namespace angreal::interpreter::environment