//
// Created by bichlmaier on 05.02.2020.
//
#include <gtest/gtest.h>
#include <string>

#include "token.h"
#include "lexer/lexer.h"

#include "parser/parser.h"
#include "parser/ast.h"

using namespace tb_lang;
using namespace tb_lang::lex;
using namespace tb_lang::parser;

typedef std::vector<Token> tokens_t;

class ParserTest : public ::testing::Test {

 protected:
  auto lexAndParseProgram(const std::string &program) {
    tokens = lexer.lex(program);
    return parser.parseProgram(tokens);
  }

  auto lexAndParseExpression(const std::string &expression) {
    tokens = lexer.lex(expression);
    return parser.parseExpression(tokens);
  }

  void printLexerResult() {
    for (auto t : tokens) {
      std::cout << t << std::endl;
    }
  }

  std::string defaultIdentifier{"x"};
  Parser parser;
  Lexer lexer;
  tokens_t tokens;
};

class VariableDeclarationTest : public ParserTest {

 protected:

  template<typename Expression>
  void parseDeclarationDetails(const std::string &program, Expression expression) {
    auto ast = lexAndParseProgram(program);
    ASSERT_EQ(1, ast->statements.size());
    auto declaration = std::dynamic_pointer_cast<AST::Declaration>(ast->statements[0]);
    ASSERT_NE(nullptr, declaration);
    EXPECT_EQ(defaultIdentifier, declaration->identifier);
    auto expr = std::dynamic_pointer_cast<Expression>(declaration->expression);
    ASSERT_NE(nullptr, expr);
    EXPECT_EQ(expression, *expr);
  }
};

class VariableAssignmentTest : public ParserTest {

 protected:

  template<typename Expression>
  void parseAssignmentDetails(const std::string &program, Expression expression) {
    auto ast = lexAndParseProgram(program);
    ASSERT_EQ(1, ast->statements.size());
    auto assignment = std::dynamic_pointer_cast<AST::Assignment>(ast->statements[0]);
    ASSERT_NE(nullptr, assignment);
    EXPECT_EQ(defaultIdentifier, assignment->identifier);
    auto expr = std::dynamic_pointer_cast<Expression>(assignment->expression);
    ASSERT_NE(nullptr, expr);
    EXPECT_EQ(expression, *expr);
  }
};

class ExpressionTest : public ParserTest {

 protected:
  template<typename Expression>
  void parseSingleExpression(const std::string &program, Expression expectedExpression) {
    auto expr = lexAndParseExpression(program);
    auto expression = std::dynamic_pointer_cast<Expression>(expr);
    ASSERT_NE(nullptr, expression);
    EXPECT_EQ(expectedExpression, *expression);
  }

  template<typename Expression>
  void parseUnaryOperation(const std::string &program, const std::string &opType, Expression expectedExpression) {
    auto expr = lexAndParseExpression(program);
    auto unaryOp = std::dynamic_pointer_cast<AST::UnaryOperation>(expr);
    ASSERT_NE(nullptr, unaryOp);

    auto sExpectedExpression = std::make_shared<Expression>(expectedExpression);

    ASSERT_EQ(AST::UnaryOperation::inferType(opType), unaryOp->type);

    auto expression = std::dynamic_pointer_cast<Expression>(unaryOp->expression);
    ASSERT_NE(nullptr, expression);
    EXPECT_EQ(expectedExpression, *expression);
  }

  template<typename LHS, typename RHS>
  void parseBinaryOperation(const std::string &program, LHS lhs, const std::string &opType, RHS rhs) {
    std::cout << "program: " << program << std::endl;
    auto expr = lexAndParseExpression(program);
    auto binaryOp = std::dynamic_pointer_cast<AST::BinaryOperation>(expr);
    ASSERT_NE(nullptr, binaryOp);

    auto lhs_ = std::make_shared<LHS>(lhs);
    auto rhs_ = std::make_shared<RHS>(rhs);

    auto expectedType = AST::BinaryOperation::inferType(opType);
    EXPECT_EQ(expectedType, binaryOp->type);

    auto actual_lhs = std::dynamic_pointer_cast<LHS>(binaryOp->lhs);
    auto actual_rhs = std::dynamic_pointer_cast<RHS>(binaryOp->rhs);

    ASSERT_NE(nullptr, actual_lhs);
    EXPECT_EQ(lhs, *actual_lhs);

    ASSERT_NE(nullptr, actual_rhs);
    EXPECT_EQ(rhs, *actual_rhs);
  }
};

class FunctionCallTest : public ParserTest {

 protected:

  template<typename Arg, typename ...ArgType>
  void checkArg(size_t index, AST::expressions_t &args, Arg expected, ArgType...ts) {
    auto actual = std::dynamic_pointer_cast<Arg>(args[index]);
    EXPECT_EQ(expected, *actual);

    if constexpr(sizeof...(ts) > 0) {
      checkArg(++index, args, ts...);
    }
  }

  void checkArg(size_t index, AST::expressions_t &args) {
    EXPECT_TRUE(args.empty());
  }

  template<typename ... ArgType>
  void parseFunction(const std::string &program, std::string identifier, ArgType ... args) {
    size_t n = sizeof ...(args);
    auto expr = lexAndParseExpression(program);
    auto functionCall = std::dynamic_pointer_cast<AST::FunctionCall>(expr);
    ASSERT_NE(nullptr, functionCall);
    EXPECT_EQ(identifier, functionCall->identifier);

    ASSERT_EQ(n, functionCall->args.size());

    size_t index{0};
    checkArg(index, functionCall->args, args...);
  }

};

class FunctionDefTest : public ParserTest {

 protected:
  void parseFunctionDef(const std::string &program, std::string identifier, TypeSystem::Type type) {
    auto prog = lexAndParseProgram(program);

    ASSERT_FALSE(prog->statements.empty());

    auto functionDef = std::dynamic_pointer_cast<AST::FunctionDeclaration>(prog->statements[0]);
    ASSERT_NE(nullptr, functionDef);

    ASSERT_TRUE(functionDef->statements.empty());
  }

  void parseFunctionDef(const std::string &program, std::string identifier, TypeSystem::Type type,
                        AST::formal_parameters parameters) {
    auto prog = lexAndParseProgram(program);

    ASSERT_FALSE(prog->statements.empty());

    auto functionDef = std::dynamic_pointer_cast<AST::FunctionDeclaration>(prog->statements[0]);
    ASSERT_NE(nullptr, functionDef);

    ASSERT_EQ(parameters.size(), functionDef->parameters.size());
    for (size_t i = 0; i < parameters.size(); ++i) {
      EXPECT_EQ(*parameters[i], *functionDef->parameters[i]);
    }

  }
};

class BlockTest : public ParserTest {

 protected:

  void parseBlock(const std::string &program, AST::Block expected) {
    auto prog = lexAndParseProgram(program);

    ASSERT_FALSE(prog->statements.empty());

    auto block = std::dynamic_pointer_cast<AST::Block>(prog->statements[0]);
    ASSERT_NE(nullptr, block);

    ASSERT_EQ(expected.statements.size(), block->statements.size());

  }
};

TEST_F(ParserTest, Empty) {
  tokens_t program = {{"", Token::Type::EndOfProgram}};
  auto ast = parser.parseProgram(program);
  ASSERT_TRUE(ast->statements.empty());
}

TEST_F(ParserTest, NewLines) {
  tokens_t program = {
      {"\n", Token::Type::NewLine},
      {"\n", Token::Type::NewLine},
      {"", Token::Type::EndOfProgram}
  };
  auto ast = parser.parseProgram(program);
  ASSERT_TRUE(ast->statements.empty());
}

TEST_F(ExpressionTest, SimpleFactors) {
  parseSingleExpression<AST::IntLiteral>("123", {123});
  parseSingleExpression<AST::FloatLiteral>("123.456", {123.456f});
  parseSingleExpression<AST::StringLiteral>("\"123\"", {"\"123\""});
  parseSingleExpression<AST::BoolLiteral>("false", {false});
  parseSingleExpression<AST::IdentifierLiteral>("x", {"x"});
}

TEST_F(ExpressionTest, SubExpressions) {
  parseSingleExpression<AST::IdentifierLiteral>("(x)", {"x"});
  parseSingleExpression<AST::IdentifierLiteral>("((x))", {"x"});
}

TEST_F(ExpressionTest, UnaryOp) {

  parseUnaryOperation<AST::IntLiteral>("+1", "+", {1});
  parseUnaryOperation<AST::FloatLiteral>("-2.0", "-", {2.0f});
  parseUnaryOperation<AST::IdentifierLiteral>("!x", "!", {"x"});
}

TEST_F(ExpressionTest, BinaryOpMultiplicative) {

  parseBinaryOperation<AST::IdentifierLiteral,
                       AST::IntLiteral>("x*1",
                                        {"x"},
                                        "*",
                                        {1}
  );

  parseBinaryOperation<AST::FloatLiteral,
                       AST::IdentifierLiteral>("1.5 / a",
                                               {1.5f},
                                               "/",
                                               {"a"}
  );

  parseBinaryOperation<AST::BoolLiteral,
                       AST::IdentifierLiteral>("true or a",
                                               {true},
                                               "or",
                                               {"a"}
  );

}

TEST_F(ExpressionTest, BinaryOpAdditive) {
  parseBinaryOperation<AST::BoolLiteral,
                       AST::BoolLiteral>("true and false",
                                         {true},
                                         "and",
                                         {false}
  );

  parseBinaryOperation<AST::FloatLiteral,
                       AST::StringLiteral>("1.4 + \"123\"",
                                           {1.4f},
                                           "+",
                                           {"\"123\""}
  );

  parseBinaryOperation<AST::IdentifierLiteral,
                       AST::IdentifierLiteral>("a - b",
                                               {"a"},
                                               "-",
                                               {"b"}
  );
}

TEST_F(ExpressionTest, BinaryOpRelational) {

  parseBinaryOperation<AST::BoolLiteral,
                       AST::BoolLiteral>("true == false",
                                         {true},
                                         "==",
                                         {false}
  );

  parseBinaryOperation<AST::FloatLiteral,
                       AST::StringLiteral>("1.4 != \"123\"",
                                           {1.4f},
                                           "!=",
                                           {"\"123\""}
  );
}

TEST_F(ExpressionTest, NestedExpressions) {
  ASSERT_NO_THROW(lexAndParseExpression("(a-b)*foo() and bar(1,false)"));
}

TEST_F(FunctionCallTest, FunctionCallTest) {
  parseFunction<>("foo()", "foo");
}

TEST_F(FunctionCallTest, FunctionCallDifferentArgs) {
  parseFunction<>("foo()", "foo");

  parseFunction<AST::IdentifierLiteral>("foo(bar)", "foo", {"bar"});

  parseFunction<AST::IntLiteral,
                AST::BoolLiteral>(
      "foo(1, true)",
      "foo",
      {1},
      {true}
  );

  parseFunction<AST::FloatLiteral,
                AST::StringLiteral,
                AST::IdentifierLiteral>(
      "foo(1.0, \"Hi\", bar)",
      "foo",
      {1.0},
      {"\"Hi\""},
      {"bar"}
  );

  parseFunction<AST::FloatLiteral,
                AST::StringLiteral,
                AST::IdentifierLiteral,
                AST::BoolLiteral>(
      "foo(1.0, \"Hi\", bar, true)",
      "foo",
      {1.0},
      {"\"Hi\""},
      {"bar"},
      {true}
  );
}

TEST_F(FunctionDefTest, FunctionDeclaration) {
  parseFunctionDef("def foo(): int {}", "foo", TypeSystem::Type::Int, {});
}

TEST_F(FunctionDefTest, FunctionDeclarationWithArgs) {
  parseFunctionDef("def foo(x: int): int {}", "foo", TypeSystem::Type::Int,
                   {
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::Int, "x"),
                   });

  parseFunctionDef("def foo(x: bool, y: string): float {}", "foo", TypeSystem::Type::Float,
                   {
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::Bool, "x"),
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::String, "y"),
                   });

  parseFunctionDef("def foo(x: int, y: float, _abc: string, sure: bool): bool {}", "foo", TypeSystem::Type::Bool,
                   {
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::Int, "x"),
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::Float, "y"),
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::String, "_abc"),
                       std::make_shared<AST::FormalParameter>(TypeSystem::Type::Bool, "sure"),
                   });
}

TEST_F(BlockTest, SimpleBlock) {
  parseBlock("{}", AST::Block({}));
}

TEST_F(VariableDeclarationTest, SimpleDeclaration) {

  parseDeclarationDetails<AST::BoolLiteral>("var x: bool = true;", {true});
  parseDeclarationDetails<AST::BoolLiteral>("var x: bool = false;", {false});
  parseDeclarationDetails<AST::IntLiteral>("var x: int = 42;", {42});
  parseDeclarationDetails<AST::FloatLiteral>("var x: float = 1.23;", {1.23f});
  parseDeclarationDetails<AST::StringLiteral>("var x: string = \"abc\";", {"\"abc\""});
  parseDeclarationDetails<AST::IdentifierLiteral>("var x: string = y;", {"y"});
}

TEST_F(VariableDeclarationTest, BoolDeclarationErrors) {
  std::vector<std::string> erroneous_program = {
      "var x bool = true;",
      "var x: bool1 = true;",
      "var 123: bool = true;",
      "var x: bool true;",
  };
  for (auto t : erroneous_program) {
    EXPECT_THROW(lexAndParseProgram(t), std::runtime_error);
  }
}

TEST_F(VariableDeclarationTest, IntDeclarationErrors) {
  std::vector<std::string> erroneous_program = {
      "var x int = 1;",
      "var x: int1 = 1;",
      "var 123: int = 1;",
      "var x: int 1;",
  };
  for (auto t : erroneous_program) {
    EXPECT_THROW(lexAndParseProgram(t), std::runtime_error);
  }
}

TEST_F(VariableDeclarationTest, FloatDeclarationErrors) {
  std::vector<std::string> erroneous_program = {
      "var x float = 1.23;",
      "var x: float1 = 1.23;",
      "var 123: float = 1.23;",
      "var x: float 1.23;",
  };
  for (auto t : erroneous_program) {
    EXPECT_THROW(lexAndParseProgram(t), std::runtime_error);
  }
}

TEST_F(VariableDeclarationTest, StringDeclarationErrors) {
  std::vector<std::string> erroneous_program = {
      "var x string = 1.23;",
      "var x: string1 = 1.23;",
      "var 123: string = 1.23;",
      "var x: string 1.23;",
  };
  for (auto t : erroneous_program) {
    EXPECT_THROW(lexAndParseProgram(t), std::runtime_error);
  }
}

TEST_F(VariableAssignmentTest, SimpleAssignment) {
  parseAssignmentDetails<AST::IntLiteral>("set x = 123;", {123});
  parseAssignmentDetails<AST::FloatLiteral>("set x = 123.456;", {123.456f});
  parseAssignmentDetails<AST::BoolLiteral>("set x = true;", {true});
  parseAssignmentDetails<AST::StringLiteral>("set x = \"123\";", {"\"123\""});
  parseAssignmentDetails<AST::IdentifierLiteral>("set x = y;", {"y"});
}


