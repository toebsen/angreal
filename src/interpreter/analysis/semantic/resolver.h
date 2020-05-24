//
// Created by toebs on 18.04.2020.
//

#ifndef ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_
#define ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_

#include <unordered_map>
#include <vector>

#include "ast_interfaces.h"
#include "common.h"

namespace angreal::interpreter::analysis {

class SemanticAnalyzer;
class Resolver {
   public:
    enum class FunctionType { None, Function, Initializer, Method };
    enum class ClassType { None, Class, SubClass };

    explicit Resolver(SemanticAnalyzer& semantic_analyzer);

    void ResolveLocal(const string_t& name, const node_t& expr);
    void ResolveFunction(
        const std::shared_ptr<FunctionDeclaration>& function_decl,
        const FunctionType& function_type);

    void ResolveClass(const std::shared_ptr<ClassDeclaration>& class_decl);

    void EnterScope();
    void LeaveScope();

    void Declare(const string_t& name);
    void Define(const string_t& name);

    void CheckAlreadyDefined(const string_t& name);

    [[nodiscard]] bool IsNoFunction() const;
    [[nodiscard]] bool IsInitializer() const;

    [[nodiscard]] bool IsNoClass() const;
    [[nodiscard]] bool IsSubClass() const;

    void Inject(const string_t& name);

   private:
    std::vector<std::unordered_map<string_t, bool>> scopes_;
    SemanticAnalyzer& semantic_analyzer_;
    FunctionType function_type_;
    ClassType class_type_;
};
}  // namespace angreal::interpreter::analysis
#endif  // ANGREAL_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_
