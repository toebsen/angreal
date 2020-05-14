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

}  // namespace angreal::interpreter::environment