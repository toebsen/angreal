//
// Created by toebs on 24.05.2020.
//

#include "error_reporter.h"

#include <iostream>

#include "ast_interfaces.h"
#include "common.h"

namespace angreal {

bool ErrorReporter::HasError() { return has_error_; }

std::string ErrorReporter::LastError() { return last_error_; };

void ErrorReporter::ParserError(const angreal::string_t& error) {
    Write("ERROR: " + error);
}

void ErrorReporter::ParserError(const string_t& error, const Token& token) {
    std::stringstream ss;
    ss << "('" << token.value() << "')";
    ss << " in line: " << std::to_string(token.position().line);
    ParserError(ss.str());
}

void ErrorReporter::AnalysisError(const angreal::string_t& error) {
    Write(error);
}

void ErrorReporter::AnalysisError(const string_t& error, node_t node) {
    AnalysisError(GetLine(node) + " | " + error);
}

void ErrorReporter::RuntimeError(const angreal::string_t& error) {
    Write(error);
    throw angreal::RuntimeError(error);
}

void ErrorReporter::RuntimeError(const string_t& error, node_t node) {
    RuntimeError(GetLine(node) + " | " + error);
}

void ErrorReporter::HandleCrucialError(const string_t& error) {
    std::cerr << error << std::endl;
}

string_t ErrorReporter::GetLine(node_t node) {
    const auto ln = node->GetLine();
    if (ln) {
        return "Line: " + std::to_string(ln.value()) + "";
    }
    return "";
}

void ErrorReporter::Write(const string_t& error) {
    std::cout << error << std::endl;

    last_error_ = error;
    has_error_ = true;
}

}  // namespace angreal