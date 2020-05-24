//
// Created by toebs on 24.05.2020.
//

#include "error_reporter.h"

#include <iostream>

#include "exceptions.h"

namespace angreal {

bool ErrorReporter::HasError() { return has_error_; }

void ErrorReporter::ParserError(const angreal::string_t& error) {
    std::cout << error;
    has_error_ = true;
}

void ErrorReporter::AnalysisError(const angreal::string_t& error) {
    std::cout << error;
    has_error_ = true;
}

void ErrorReporter::RuntimeError(const angreal::string_t& error) {
    std::cout << error;
    has_error_ = true;

    throw angreal::RuntimeError(error);
}

void ErrorReporter::HandleCrucialError(const string_t& error) {
    std::cerr << error;
}

}  // namespace angreal