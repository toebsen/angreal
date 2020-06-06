//
// Created by toebs on 24.05.2020.
//

#ifndef ANGREAL_SRC_COMMON_ERROR_HANDLER_H_
#define ANGREAL_SRC_COMMON_ERROR_HANDLER_H_

#include "common.h"
#include "token.h"

namespace angreal {

using namespace parser::AST;

class IErrorHandler {
   public:
    virtual bool HasError() = 0;
    virtual std::string LastError() = 0;

    virtual void ParserError(const string_t& error) = 0;
    virtual void ParserError(const string_t& error, const Token& token) = 0;

    virtual void AnalysisError(const string_t& error) = 0;
    virtual void AnalysisError(const string_t& error, node_t node) = 0;

    virtual void RuntimeError(const string_t& error) = 0;
    virtual void RuntimeError(const string_t& error, node_t node) = 0;

    virtual void HandleCrucialError(const string_t& error) = 0;
};

using error_handler_t = std::shared_ptr<IErrorHandler>;

}  // namespace angreal

#endif  // ANGREAL_SRC_COMMON_ERROR_HANDLER_H_
