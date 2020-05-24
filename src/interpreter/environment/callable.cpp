//
// Created by toebs on 11.04.2020.
//

#include "callable.h"

#include "../interpreter.h"

namespace angreal::interpreter::environment {

const string_t Class::kInitializerName = "init";

Function::Function(std::shared_ptr<FunctionDeclaration> function_decl,
                   environment_t env)
    : function_decl_(std::move(function_decl)),
      env_(std::move(env)),
      is_initializer_ {function_decl_->identifier == Class::kInitializerName} {}

bool Function::CheckArity(const std::vector<obj_t>& args) const {
    return args.size() == Arity();
}

size_t Function::Arity() const { return function_decl_->parameters.size(); }

std::optional<obj_t> Function::Call(const interpreter_t& interp,
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
        if (is_initializer_) {
            return env_->Get("self", 0);
        }
        return ret;
    }

    return std::nullopt;
}

string_t Function::Stringify() const {
    return "function(" + function_decl_->identifier + ")";
}

obj_t Function::Bind(const obj_t& instance) {
    auto bound_env = std::make_shared<Environment>(env_);
    bound_env->Declare("self", instance);
    auto fun = std::make_shared<Function>(function_decl_, bound_env);
    auto type = std::make_shared<Type>(fun);

    return std::make_shared<Object>(type);
}

Class::Class(std::shared_ptr<ClassDeclaration> class_declaration,
             std::unordered_map<string_t, obj_t> methods,
             std::optional<obj_t> superclass, environment_t env)
    : class_declaration_(std::move(class_declaration)),
      methods_(std::move(methods)),
      superclass_(std::move(superclass)),
      env_(std::move(env)),
      initializer_() {}

bool Class::CheckArity(const std::vector<obj_t>& args) const {
    if (initializer_) {
        return initializer_->CheckArity(args);
    }
    return true;
}

size_t Class::Arity() const {
    if (initializer_) {
        return initializer_->Arity();
    }
    return 0;
}

std::optional<obj_t> Class::Call(interpreter_t const& interp,
                                 const std::vector<obj_t>& args) {
    instance_t instance = std::make_shared<Instance>(shared_from_this());
    auto type = std::make_shared<Type>(instance);
    auto default_instance = std::make_shared<Object>(type);

    if (FindMethod(kInitializerName)) {
        auto bound =
            default_instance->GetType()->AsInstance()->Get(kInitializerName);
        initializer_ = bound->GetType()->AsCallable();
        initializer_->Call(interp, args);
    }

    return default_instance;
}

string_t Class::Stringify() const {
    return "class(" + class_declaration_->identifier + ")";
}

std::optional<obj_t> Class::FindMethod(const string_t& name) const {
    if (methods_.contains(name)) {
        return methods_.at(name);
    }

    if (superclass_.has_value()) {
        auto superclass = std::dynamic_pointer_cast<Class>(
            superclass_.value()->GetType()->AsCallable());
        if (auto super_method = superclass->FindMethod(name)) {
            return super_method;
        }
    }

    return std::nullopt;
}

Instance::Instance(std::shared_ptr<Class> _class) : class_(std::move(_class)) {}

string_t Instance::Stringify() const {
    return "Instance of " + class_->Stringify();
}

obj_t Instance::Get(const string_t& name) {
    if (fields_.contains(name)) {
        return fields_[name];
    }

    if (auto method = class_->FindMethod(name)) {
        auto unbound = method.value();
        auto bound = std::dynamic_pointer_cast<Function>(
                         unbound->GetType()->AsCallable())
                         ->Bind(AsObject());
        return bound;
    }

    throw RuntimeError("Undefined property <" + name + "> of " +
                       class_->Stringify());
}

void Instance::Set(const string_t& name, const obj_t& value) {
    fields_[name] = value;
}

obj_t Instance::AsObject() {
    return std::make_shared<Object>(std::make_shared<Type>(shared_from_this()));
}

}  // namespace angreal::interpreter::environment