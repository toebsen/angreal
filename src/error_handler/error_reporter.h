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

    void ParserError(const string_t& error) override;

    void AnalysisError(const string_t& error) override;

    void RuntimeError(const string_t& error) override;

    void HandleCrucialError(const string_t& error) override;

   private:
    bool has_error_ {false};
};

}  // namespace angreal

#endif  // ANGREAL_SRC_ERROR_HANDLER_ERRORREPORTER_H_
