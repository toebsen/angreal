//
// Created by toebs on 11.04.2020.
//

#include "callable.h"

#include "../interpreter.h"
#include "environment.h"
#include "object.h"

namespace tb_lang {
namespace interpreter {
namespace environment {

type_t FromType(parser::TypeSystem::Type type) {
    switch (type) {
        case parser::TypeSystem::Type::Bool:
            return std::make_shared<BoolType>(false);
        case parser::TypeSystem::Type::Int:
            return std::make_shared<IntType>(0);
        case parser::TypeSystem::Type::Float:
            return std::make_shared<FloatType>(0);
        case parser::TypeSystem::Type::String:
            return std::make_shared<StringType>("");
    }
    return type_t(nullptr);
}

Function::Function(const std::shared_ptr<FunctionDeclaration>& function_decl,
                   const environment_t& env)
    : function_decl_(function_decl), env_(env) {}

bool Function::CheckArity(const std::vector<obj_t>& args) const {
    return args.size() == function_decl_->parameters.size();
}

size_t Function::Arity(void) const { return function_decl_->parameters.size(); }

obj_t Function::Call(const interpreter_t& interp,
                     const std::vector<obj_t>& args) {
    auto local_env = std::make_shared<Environment>(*env_);

    if (CheckArity(args) == false) {
        std::stringstream ss;
        ss << "Called function <" << function_decl_->identifier
           << "> with wrong args";
        ss << "Expected <" << Arity() << "> but got: " << args.size()
           << std::endl;
        throw std::runtime_error(ss.str());
    }

    for (size_t kI = 0; kI < args.size(); ++kI) {
        auto param_decl = function_decl_->parameters[kI];
        auto param_type = FromType(param_decl->type);
        if (!args[kI]->GetType()->HasSameType(*param_type)) {
            std::stringstream ss;
            ss << "Called function <" << function_decl_->identifier
               << "> with wrongly typed arg";
            ss << "Expected <" << param_decl->identifier << std::endl;
            throw std::runtime_error(ss.str());
        }

        local_env->Declare(function_decl_->parameters[kI]->identifier,
                           args[kI]);
    }

    auto ret_obj = interp->invoke(function_decl_->statements, local_env);
    return ret_obj;
}

type_t Function::ReturnType() const { return FromType(function_decl_->type); }

string_t Function::Stringify(void) const { return tb_lang::string_t(); }

}  // namespace environment
}  // namespace interpreter
}  // namespace tb_lang