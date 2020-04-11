//
// Created by toebs on 11.04.2020.
//

#include "callable.h"
#include "object.h"
#include "environment.h"
#include "../interpreter.h"

namespace tb_lang {
namespace interpreter {
namespace environment {

Function::Function(const std::shared_ptr<FunctionDeclaration>& function_decl,
                   const environment_t& env)
    : function_decl_(function_decl), env_(env) {}


bool Function::CheckArity(const std::vector<obj_t>& args) const {
    return args.size() == function_decl_->parameters.size();
}

size_t Function::Arity(void) const {
    return function_decl_->parameters.size();
}

obj_t Function::Call(const interpreter_t& interp,
                     const std::vector<obj_t>& args) {

    auto local_env = std::make_shared<Environment>(*env_);
    if(CheckArity(args))
    {
        for (size_t kI = 0; kI < args.size(); ++kI) {
            // Todo: missing type check
            local_env->Declare(function_decl_->parameters[kI]->identifier, args[kI]);
        }
    }
    try {
        return interp->invoke(function_decl_->statements, local_env);
    }
    catch(...)
    {

    }

    return obj_t(nullptr);
}
string_t Function::Stringify(void) const { return tb_lang::string_t(); }

}  // namespace environment
}  // namespace interpreter
}  // namespace tb_lang