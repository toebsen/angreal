//
// Created by toebs on 11.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
#define TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_


#include "../parser/ast.h"

namespace tb_lang {
namespace interpreter {

class Interpreter;
using interpreter_t = Interpreter*;

namespace environment {

class Object;
using obj_t = std::shared_ptr<Object>;

class Environment;
using environment_t = std::shared_ptr<Environment>;

class Callable : private NonCopyable {
   public:
    virtual bool CheckArity(const std::vector<obj_t>& args) const = 0;
    virtual size_t Arity() const = 0;

    virtual obj_t Call(const interpreter_t& interp,
                       const std::vector<obj_t>& args) = 0;
    virtual string_t Stringify(void) const = 0;
};


class Function final : public Callable,
                       public std::enable_shared_from_this<Function>
{
   public:
    Function(const std::shared_ptr<FunctionDeclaration>& function_decl,
        const environment_t& env
        );

    bool CheckArity(const std::vector<obj_t>& args) const override;

    size_t Arity() const override;

    obj_t Call(const interpreter_t& interp,
                       const std::vector<obj_t>& args) override ;

    string_t Stringify(void) const override ;

   private:
    std::shared_ptr<FunctionDeclaration> function_decl_;
    environment_t env_;

};

}  // namespace environment
}  // namespace interpreter
}  // namespace tb_lang
#endif  // TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
