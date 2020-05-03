//
// Created by toebs on 18.04.2020.
//

#include "resolver.h"

#include "ast.h"
#include "semantic_analyzer.h"

namespace tb_lang {

namespace interpreter {
namespace analysis {

void Resolver::Declare(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        if (scope.find(name) != scope.end()) {
            throw RuntimeError("variable `" + name +
                               "` already declared in this scope");
        } else {
            scope[name] = false;
        }
    }
}

void Resolver::Define(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        scope[name] = true;
    }
}

void Resolver::EnterScope(void) {
    scopes_.push_back({});
}

void Resolver::LeaveScope(void) {
    scopes_.pop_back();
}

void Resolver::ResolveLocal(const string_t& name, const node_t& expr) {
    for (auto s=scopes_.rbegin(); s!=scopes_.rend(); ++s) {
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
    const std::shared_ptr<FunctionDeclaration>& function_decl) {
    FunctionType enclosing_function = function_type_;
    function_type_ = FunctionType::Function;
    EnterScope();
    for (auto param : function_decl->parameters) {
        Declare(param->identifier);
        Define(param->identifier);
    }
    semantic_analyzer_.Resolve(function_decl->statements);
    LeaveScope();
    function_type_ = enclosing_function;
}

}  // namespace analysis
}  // namespace interpreter
}  // namespace tb_lang