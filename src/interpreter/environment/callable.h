//
// Created by toebs on 11.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
#define TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_

#include "../parser/ast.h"

namespace tb_lang::interpreter {

class Interpreter;
using interpreter_t = Interpreter*;

namespace environment {

class ICallable : private NonCopyable {
   public:
    [[nodiscard]] virtual bool CheckArity(
        const std::vector<obj_t>& args) const = 0;

    [[nodiscard]] virtual size_t Arity() const = 0;

    virtual obj_t Call(const interpreter_t& interp,
                       const std::vector<obj_t>& args) = 0;

    [[nodiscard]] virtual string_t Stringify() const = 0;
};

class Function final : public ICallable,
                       public std::enable_shared_from_this<Function> {
   public:
    Function(std::shared_ptr<FunctionDeclaration> function_decl,
             environment_t env);

    bool CheckArity(const std::vector<obj_t>& args) const override;

    size_t Arity() const override;

    obj_t Call(const interpreter_t& interp,
               const std::vector<obj_t>& args) override;

    string_t Stringify() const override;

   private:
    std::shared_ptr<FunctionDeclaration> function_decl_;
    environment_t env_;
};

}  // namespace environment
}  // namespace tb_lang::interpreter
#endif  // TBLANG_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
