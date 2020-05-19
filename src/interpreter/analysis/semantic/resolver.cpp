//
// Created by toebs on 18.04.2020.
//

#include "resolver.h"

#include "ast.h"
#include "semantic_analyzer.h"

namespace angreal::interpreter::analysis {

Resolver::Resolver(SemanticAnalyzer& semantic_analyzer)
    : semantic_analyzer_(semantic_analyzer),
      function_type_{FunctionType::None} {}

void Resolver::Declare(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        if (scope.find(name) != scope.end()) {
            throw RuntimeError("variable `" + name +
                               "` already declared in this scope");
        }

        scope[name] = false;
    }
}

void Resolver::Define(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        scope[name] = true;
    }
}

void Resolver::EnterScope() { scopes_.emplace_back(); }

void Resolver::LeaveScope() { scopes_.pop_back(); }

void Resolver::ResolveLocal(const string_t& name, const node_t& expr) {
    for (auto s = scopes_.rbegin(); s != scopes_.rend(); ++s) {
        if (s->find(name) != s->end()) {
            auto distance = std::distance(scopes_.rbegin(), s);
            semantic_analyzer_.ResolveLocal(expr, distance);
            return;
        }
    }
}
void Resolver::CheckAlreadyDefined(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        if (scope.find(name) != scope.end() && !scope.contains(name)) {
            throw RuntimeError("cannot read local variable `" + name +
                               "` in its own initializer");
        }
    }
}

void Resolver::ResolveFunction(
    const std::shared_ptr<FunctionDeclaration>& function_decl,
    const FunctionType& function_type) {
    FunctionType enclosing_function = function_type_;
    function_type_ = function_type;
    EnterScope();
    for (const auto& param : function_decl->parameters) {
        Declare(param->identifier);
        Define(param->identifier);
    }
    semantic_analyzer_.Resolve(function_decl->statements);
    LeaveScope();
    function_type_ = enclosing_function;
}

bool Resolver::IsFunction() { return !(function_type_ == FunctionType::None); }

}  // namespace angreal::interpreter::analysis