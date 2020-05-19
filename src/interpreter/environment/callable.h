//
// Created by toebs on 11.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
#define ANGREAL_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_

#include <unordered_map>

#include "../parser/ast.h"

namespace angreal::interpreter {

class Interpreter;
using interpreter_t = Interpreter*;

namespace environment {

class ICallable {
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

class Class final : public ICallable,
                    public std::enable_shared_from_this<Class> {
   public:
    Class(std::shared_ptr<ClassDeclaration> class_declaration,
          std::unordered_map<string_t, obj_t> methods, environment_t env);

    bool CheckArity(const std::vector<obj_t>& args) const override;

    size_t Arity() const override;

    obj_t Call(const interpreter_t& interp,
               const std::vector<obj_t>& args) override;

    std::optional<obj_t> FindMethod(string_t name);

    string_t Stringify() const override;

   private:
    std::shared_ptr<ClassDeclaration> class_declaration_;
    std::unordered_map<string_t, obj_t> methods_;
    environment_t env_;
};

class Instance final {
   public:
    explicit Instance(std::shared_ptr<Class> _class);

    string_t Stringify() const;

    obj_t Get(const string_t& name);

    void Set(const string_t& name, const obj_t& value);

   private:
    std::shared_ptr<Class> class_;
    std::unordered_map<string_t, obj_t> fields_;
};
}  // namespace environment
}  // namespace angreal::interpreter
#endif  // ANGREAL_SRC_INTERPRETER_ENVIRONMENT_CALLABLE_H_
