//
// Created by toebs on 11.04.2020.
//

#ifndef TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
#define TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "context.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "lexer.h"

using namespace tb_lang;
using namespace tb_lang::lex;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;
using namespace tb_lang::interpreter::environment;

class BaseTest : public ::testing::Test {
   public:

   protected:
    Context context_;

};

#endif  // TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
