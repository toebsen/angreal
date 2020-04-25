//
// Created by toebs on 11.04.2020.
//

#ifndef TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
#define TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "context.h"
#include "lexer.h"
#include "parser/ast.h"
#include "parser/parser.h"

using namespace tb_lang;
using namespace tb_lang::lex;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;
using namespace tb_lang::interpreter::environment;

class BaseTest : public ::testing::Test {
   public:

    void SafeRun(const std::string& code, const std::string& expected) {
        try {
            testing::internal::CaptureStdout();
            context_.interpreter->interpret(code);
            std::string out = testing::internal::GetCapturedStdout();
            ASSERT_EQ(expected + "\n", out);
        } catch (const std::exception& e) {
            testing::internal::GetCapturedStdout();
            FAIL() << "Exception not expected:" << e.what();
        }
    }

    void ExpectRuntimeException(const std::string& code,
                                const std::string& expected_exception) {
        ASSERT_THROW(
            {
                try {
                    context_.interpreter->interpret(code);
                    FAIL() << "Expected RuntimeError with message: "
                           << expected_exception;
                } catch (const RuntimeError& e) {
                    EXPECT_EQ(expected_exception, e.what());
                    throw;
                } catch (...) {
                    FAIL() << "Expected RuntimeError with message: "
                           << expected_exception;
                }
            },
            RuntimeError);
    }

   protected:
    Context context_;
};

#endif  // TBLANG_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
