//
// Created by toebs on 18.04.2020.
//

#ifndef TBLANG_TESTS_INTERPRETER_SEMANTIC_ANALYZER_TEST_FIXTURES_H_
#define TBLANG_TESTS_INTERPRETER_SEMANTIC_ANALYZER_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "analysis/semantic/semantic_analyzer.h"

#include "parser/ast.h"

using namespace tb_lang;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;
using namespace tb_lang::interpreter::analysis;

class SemanticAnalyzerTest : public ::testing::Test {
   protected:
    SemanticAnalyzer semantic_analyzer_;
};

#endif  // TBLANG_TESTS_INTERPRETER_SEMANTIC_ANALYZER_TEST_FIXTURES_H_
