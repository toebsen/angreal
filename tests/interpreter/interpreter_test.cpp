//
// Created by toebs on 29.03.2020.
//

#include <gtest/gtest.h>
#include <string>

#include "token.h"
#include "lexer/lexer.h"

#include "parser/parser.h"
#include "parser/ast.h"

#include "interpreter/interpreter.h"
#include "interpreter/scope.h"

using namespace tb_lang;
using namespace tb_lang::lex;
using namespace tb_lang::parser;
using namespace tb_lang::interpreter;

TEST(InterpreterTest, Declaration)
{
  Scope scope;
  Interpreter interpreter(scope);

  ValueLiteral<std::string> hello_world{ "Hello World!" };
  AST::Declaration declaration{
    TypeSystem::Type::String,
    "test",
    std::make_shared<ValueLiteral<std::string>>(hello_world)
  };

  interpreter.visit(&declaration);
  ASSERT_EQ(scope.value("test"), "Hello World!");

}