//
// Created by toebs on 18.04.2020.
//

#ifndef TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_
#define TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_

#include <unordered_map>
#include <vector>

#include "ast_interfaces.h"
#include "common.h"

namespace tb_lang {
namespace interpreter {
namespace analysis {

class SemanticAnalyzer;
class Resolver {

   enum class FunctionType {
        None,
        Function
    };

   public:
    Resolver(SemanticAnalyzer& semantic_analyzer)
        : semantic_analyzer_(semantic_analyzer) {}

    ~Resolver() = default;

    void ResolveLocal(const string_t& name, const node_t& expr);
    void ResolveFunction(
        const std::shared_ptr<FunctionDeclaration>& function_decl);

    void EnterScope(void);
    void LeaveScope(void);

    void Declare(const string_t& name);
    void Define(const string_t& name);

    void CheckAlreadyDefined(const string_t& name);

    bool IsFunction(){return function_type_ == FunctionType::Function;}

   private:
    std::vector<std::unordered_map<string_t, bool>> scopes_;
    SemanticAnalyzer& semantic_analyzer_;
    FunctionType function_type_;
};
}  // namespace analysis
}  // namespace interpreter
}  // namespace tb_lang
#endif  // TBLANG_SRC_INTERPRETER_ANALYSIS_SEMANTIC_RESOLVER_H_
