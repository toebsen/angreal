//
// Created by toebs on 18.04.2020.
//

#include "resolver.h"

#include "ast.h"
#include "semantic_analyzer.h"

namespace angreal::interpreter::analysis {

Resolver::Resolver(error_handler_t error_handler,
                   SemanticAnalyzer& semantic_analyzer)
    : error_handler_(error_handler),
      semantic_analyzer_(semantic_analyzer),
      function_type_ {FunctionType::None},
      class_type_ {ClassType::None} {}

void Resolver::Declare(const string_t& name) {
    if (!scopes_.empty()) {
        auto& scope = scopes_.back();
        if (scope.find(name) != scope.end()) {
            error_handler_->AnalysisError("variable `" + name +
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
            error_handler_->AnalysisError("cannot read local variable `" +
                                          name + "` in its own initializer");
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

void Resolver::ResolveClass(
    const std::shared_ptr<ClassDeclaration>& class_decl) {
    Declare(class_decl->identifier);
    Define(class_decl->identifier);
    ClassType enclosing_class = class_type_;
    class_type_ = ClassType::Class;

    if (class_decl->superclass) {
        class_type_ = ClassType::SubClass;
        EnterScope();
        Inject("super");
    }

    EnterScope();
    {
        Inject("self");

        for (auto method : class_decl->methods) {
            FunctionType type = method->identifier == "init"
                                    ? FunctionType::Initializer
                                    : FunctionType::Method;
            ResolveFunction(method, type);
        }
    }
    LeaveScope();

    if (class_decl->superclass) {
        LeaveScope();
    }

    class_type_ = enclosing_class;
}
bool Resolver::IsNoFunction() const {
    return function_type_ == FunctionType::None;
}

bool Resolver::IsInitializer() const {
    return function_type_ == FunctionType::Initializer;
}

void Resolver::Inject(const string_t& name) {
    scopes_.back().emplace(name, true);
}
bool Resolver::IsNoClass() const { return class_type_ == ClassType::None; }

bool Resolver::IsSubClass() const { return class_type_ == ClassType::SubClass; }

}  // namespace angreal::interpreter::analysis