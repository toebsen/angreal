//
// Created by toebs on 11.04.2020.
//

#ifndef ANGREAL_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
#define ANGREAL_TESTS_ACCEPTANCE_TEST_FIXTURES_H_

#include <gtest/gtest.h>

#include "context.h"
#include "lexer.h"
#include "parser/ast.h"
#include "parser/parser.h"

using namespace angreal;
using namespace angreal::lex;
using namespace angreal::parser;
using namespace angreal::interpreter;
using namespace angreal::interpreter::environment;

class BaseTest : public ::testing::Test {
   public:
    void SafeRun(const std::string& code, const std::string& expected) {
        try {
            testing::internal::CaptureStdout();
            context_.interpreter->interpret(code);
            std::string out = testing::internal::GetCapturedStdout();
            ASSERT_EQ(expected + "\n", out);
        } catch (const std::exception& e) {
            std::string out = testing::internal::GetCapturedStdout();
            FAIL() << "Exception not expected:" << e.what() << " with: " << out;
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

class ClassTest : public BaseTest {};
class ConditionalTest : public BaseTest {};
class ErroneousTest : public BaseTest {};
class ExampleTest : public BaseTest {};
class FunctionTest : public BaseTest {};
class ScopingTest : public BaseTest {};

#endif  // ANGREAL_TESTS_ACCEPTANCE_TEST_FIXTURES_H_
