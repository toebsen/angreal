//
// Created by toebs on 24.05.2020.
//

#ifndef ANGREAL_SRC_ERROR_HANDLER_ERRORREPORTER_H_
#define ANGREAL_SRC_ERROR_HANDLER_ERRORREPORTER_H_

#include "error_handler.h"

namespace angreal {

class ErrorReporter : public IErrorHandler {
   public:
    bool HasError() override;
    std::string LastError() override;

    void ParserError(const string_t& error) override;
    void ParserError(const string_t& error, const Token& token) override;

    void AnalysisError(const string_t& error) override;
    void AnalysisError(const string_t& error, node_t node) override;

    void RuntimeError(const string_t& error) override;
    void RuntimeError(const string_t& error, node_t node) override;

    void HandleCrucialError(const string_t& error) override;

   private:
    void Write(const string_t& error);

    string_t GetLine(node_t node);
    bool has_error_ {false};
    string_t last_error_ {""};
};

}  // namespace angreal

#endif  // ANGREAL_SRC_ERROR_HANDLER_ERRORREPORTER_H_
