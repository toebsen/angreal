//
// Created by bichlmaier on 05.02.2020.
//
#include "parser/parser.h"

#include <string>

#include <gtest/gtest.h>

#include "lexer/lexer.h"
#include "parser/ast.h"
#include "token.h"

using namespace tb_lang;
using namespace tb_lang::lex;
using namespace tb_lang::parser;

typedef std::vector<Token> tokens_t;

class ParserTest : public ::testing::Test {
   protected:
    auto lexAndParseProgram(const std::string& program) {
        tokens = lexer.lex(program);
        return parser.parseProgram(tokens);
    }

    auto lexAndParseExpression(const std::string& expression) {
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
    template <typename Expression>
    void parseDeclarationDetails(const std::string& program,
                                 expression_t expression) {
        auto ast = lexAndParseProgram(program);
        ASSERT_EQ(1, ast->statements.size());
        auto declaration =
            std::dynamic_pointer_cast<AST::Declaration>(ast->statements[0]);
        ASSERT_NE(nullptr, declaration);
        EXPECT_EQ(defaultIdentifier, declaration->identifier);
        auto expr =
            std::dynamic_pointer_cast<Expression>(declaration->expression);
        ASSERT_NE(nullptr, expr);
        EXPECT_EQ(*std::dynamic_pointer_cast<Expression>(expression), *expr);
    }
};

class VariableAssignmentTest : public ParserTest {
   protected:
    template <typename Expression>
    void parseAssignmentDetails(const std::string& program,
                                expression_t expression) {
        auto ast = lexAndParseProgram(program);
        ASSERT_EQ(1, ast->statements.size());
        auto assignment =
            std::dynamic_pointer_cast<AST::Assignment>(ast->statements[0]);
        ASSERT_NE(nullptr, assignment);
        EXPECT_EQ(defaultIdentifier, assignment->identifier);
        auto expr =
            std::dynamic_pointer_cast<Expression>(assignment->expression);
        ASSERT_NE(nullptr, expr);
        EXPECT_EQ(*std::dynamic_pointer_cast<Expression>(expression), *expr);
    }
};

class ExpressionTest : public ParserTest {
   protected:
    template <typename Expression>
    void parseSingleExpression(const std::string& program,
                               expression_t expectedExpression) {
        auto expr = lexAndParseExpression(program);
        auto expression = std::dynamic_pointer_cast<Expression>(expr);
        ASSERT_NE(nullptr, expression);
        EXPECT_EQ(*std::dynamic_pointer_cast<Expression>(expectedExpression),
                  *expression);
    }

    template <typename Expression>
    void parseUnaryOperation(const std::string& program,
                             const std::string& opType,
                             expression_t expectedExpression) {
        auto expr = lexAndParseExpression(program);
        auto unaryOp = std::dynamic_pointer_cast<AST::UnaryOperation>(expr);
        ASSERT_NE(nullptr, unaryOp);

        ASSERT_EQ(AST::UnaryOperation::inferType(opType), unaryOp->type);

        auto expression =
            std::dynamic_pointer_cast<Expression>(unaryOp->expression);
        ASSERT_NE(nullptr, expression);
        EXPECT_EQ(*std::dynamic_pointer_cast<Expression>(expectedExpression),
                  *expression);
    }

    template <typename LHS, typename RHS>
    void parseBinaryOperation(const std::string& program, expression_t lhs,
                              const std::string& opType, expression_t rhs) {
        std::cout << "program: " << program << std::endl;
        auto expr = lexAndParseExpression(program);
        auto binaryOp = std::dynamic_pointer_cast<AST::BinaryOperation>(expr);
        ASSERT_NE(nullptr, binaryOp);

        auto lhs_ = std::dynamic_pointer_cast<LHS>(lhs);
        auto rhs_ = std::dynamic_pointer_cast<RHS>(rhs);

        ASSERT_TRUE(lhs_);
        ASSERT_TRUE(rhs_);

        auto expectedType = AST::BinaryOperation::inferType(opType);
        EXPECT_EQ(expectedType, binaryOp->type);

        auto actual_lhs = std::dynamic_pointer_cast<LHS>(binaryOp->lhs);
        auto actual_rhs = std::dynamic_pointer_cast<RHS>(binaryOp->rhs);

        ASSERT_NE(nullptr, actual_lhs);
        EXPECT_EQ(*lhs_, *actual_lhs);

        ASSERT_NE(nullptr, actual_rhs);
        EXPECT_EQ(*rhs_, *actual_rhs);
    }
};

// class FunctionCallTest : public ParserTest {
//   protected:
//    template <typename Arg, typename... ArgType>
//    void checkArg(size_t index, AST::expressions_t &args, Arg expected,
//                  ArgType... ts) {
//        auto actual = std::dynamic_pointer_cast<Arg>(args[index]);
//        EXPECT_EQ(expected, *actual);
//
//        if constexpr (sizeof...(ts) > 0) {
//            checkArg(++index, args, ts...);
//        }
//    }
//
//    void checkArg(size_t index, AST::expressions_t &args) {
//        EXPECT_TRUE(args.empty());
//    }
//
//    template <typename... ArgType>
//    void parseFunction(const std::string &program, std::string identifier,
//                       ArgType... args) {
//        size_t n = sizeof...(args);
//        auto expr = lexAndParseExpression(program);
//        auto functionCall =
//        std::dynamic_pointer_cast<AST::FunctionCall>(expr); ASSERT_NE(nullptr,
//        functionCall); EXPECT_EQ(identifier, functionCall->identifier);
//
//        ASSERT_EQ(n, functionCall->args.size());
//
//        size_t index{0};
//        checkArg(index, functionCall->args, args...);
//    }
//};

class FunctionDefTest : public ParserTest {
   protected:
    void parseFunctionDef(const std::string& program, std::string identifier,
                          TypeHelper::Type type) {
        auto prog = lexAndParseProgram(program);

        ASSERT_FALSE(prog->statements.empty());

        auto functionDef = std::dynamic_pointer_cast<AST::FunctionDeclaration>(
            prog->statements[0]);
        ASSERT_NE(nullptr, functionDef);

        ASSERT_TRUE(functionDef->statements.empty());
    }

    void parseFunctionDef(const std::string& program, std::string identifier,
                          TypeHelper::Type type,
                          AST::formal_parameters parameters) {
        auto prog = lexAndParseProgram(program);

        ASSERT_FALSE(prog->statements.empty());

        auto functionDef = std::dynamic_pointer_cast<AST::FunctionDeclaration>(
            prog->statements[0]);
        ASSERT_NE(nullptr, functionDef);

        ASSERT_EQ(parameters.size(), functionDef->parameters.size());
        for (size_t i = 0; i < parameters.size(); ++i) {
            EXPECT_EQ(*parameters[i], *functionDef->parameters[i]);
        }
    }
};

class BlockTest : public ParserTest {
   protected:
    void parseBlock(const std::string& program,
                    std::shared_ptr<AST::Block> expected) {
        auto prog = lexAndParseProgram(program);

        ASSERT_FALSE(prog->statements.empty());

        auto block = std::dynamic_pointer_cast<AST::Block>(prog->statements[0]);
        ASSERT_NE(nullptr, block);

        // Todo: not sufficient test
        ASSERT_EQ(expected->statements.size(), block->statements.size());
    }
};

TEST_F(ParserTest, Empty) {
    tokens_t program = {{"", Token::Type::EndOfProgram}};
    auto ast = parser.parseProgram(program);
    ASSERT_TRUE(ast->statements.empty());
}

TEST_F(ParserTest, NewLines) {
    tokens_t program = {{"\n", Token::Type::NewLine},
                        {"\n", Token::Type::NewLine},
                        {"", Token::Type::EndOfProgram}};
    auto ast = parser.parseProgram(program);
    ASSERT_TRUE(ast->statements.empty());
}

TEST_F(ExpressionTest, SimpleFactors) {
    parseSingleExpression<AST::IntLiteral>(
        "123", std::make_shared<AST::IntLiteral>(123));
    parseSingleExpression<AST::FloatLiteral>(
        "123.456", std::make_shared<AST::FloatLiteral>(123.456f));
    parseSingleExpression<AST::StringLiteral>(
        "\"123\"", std::make_shared<AST::StringLiteral>("123"));
    parseSingleExpression<AST::BoolLiteral>(
        "false", std::make_shared<AST::BoolLiteral>(false));
    parseSingleExpression<AST::IdentifierLiteral>(
        "x", std::make_shared<AST::IdentifierLiteral>("x"));
}

TEST_F(ExpressionTest, SubExpressions) {
    parseSingleExpression<AST::IdentifierLiteral>(
        "(x)", std::make_shared<AST::IdentifierLiteral>("x"));
    parseSingleExpression<AST::IdentifierLiteral>(
        "((x))", std::make_shared<AST::IdentifierLiteral>("x"));
}

TEST_F(ExpressionTest, UnaryOp) {
    parseUnaryOperation<AST::IntLiteral>("+1", "+",
                                         std::make_shared<AST::IntLiteral>(1));
    parseUnaryOperation<AST::FloatLiteral>(
        "-2.0", "-", std::make_shared<AST::FloatLiteral>(2.0f));
    parseUnaryOperation<AST::IdentifierLiteral>(
        "!x", "!", std::make_shared<AST::IdentifierLiteral>("x"));
}

TEST_F(ExpressionTest, BinaryOpMultiplicative) {
    parseBinaryOperation<AST::IdentifierLiteral, AST::IntLiteral>(
        "x*1", std::make_shared<AST::IdentifierLiteral>("x"), "*",
        std::make_shared<AST::IntLiteral>(1));

    parseBinaryOperation<AST::FloatLiteral, AST::IdentifierLiteral>(
        "1.5 / a", std::make_shared<AST::FloatLiteral>(1.5f), "/",
        std::make_shared<AST::IdentifierLiteral>("a"));

    parseBinaryOperation<AST::BoolLiteral, AST::IdentifierLiteral>(
        "true or a", std::make_shared<AST::BoolLiteral>(true), "or",
        std::make_shared<AST::IdentifierLiteral>("a"));
}

TEST_F(ExpressionTest, BinaryOpAdditive) {
    parseBinaryOperation<AST::BoolLiteral, AST::BoolLiteral>(
        "true and false", std::make_shared<AST::BoolLiteral>(true), "and",
        std::make_shared<AST::BoolLiteral>(false));

    parseBinaryOperation<AST::FloatLiteral, AST::StringLiteral>(
        "1.4 + \"123\"", std::make_shared<AST::FloatLiteral>(1.4f), "+",
        std::make_shared<AST::StringLiteral>("123"));

    parseBinaryOperation<AST::IdentifierLiteral, AST::IdentifierLiteral>(
        " a - b ", std::make_shared<AST::IdentifierLiteral>("a"), "-",
        std::make_shared<AST::IdentifierLiteral>("b"));
}

TEST_F(ExpressionTest, BinaryOpRelational) {
    parseBinaryOperation<AST::BoolLiteral, AST::BoolLiteral>(
        " true == false ", std::make_shared<AST::BoolLiteral>(true),
        "==", std::make_shared<AST::BoolLiteral>(false));

    parseBinaryOperation<AST::FloatLiteral, AST::StringLiteral>(
        " 1.4 != \"123\" ", std::make_shared<AST::FloatLiteral>(1.4f),
        "!=", std::make_shared<AST::StringLiteral>("123"));
}

TEST_F(ExpressionTest, NestedExpressions) {
    ASSERT_NO_THROW(lexAndParseExpression("(a-b)*foo() and bar(1,false)"));
}

// TEST_F(FunctionCallTest, FunctionCallTest) { parseFunction<>("foo()", "foo");
// }
//
// TEST_F(FunctionCallTest, FunctionCallDifferentArgs) {
//    parseFunction<>("foo()", "foo");
//
//    parseFunction<AST::IdentifierLiteral>("foo(bar)", "foo", {"bar"});
//
//    parseFunction<AST::IntLiteral, AST::BoolLiteral>("foo(1, true)", "foo",
//    {1},
//                                                     {true});
//
//    parseFunction<AST::FloatLiteral, AST::StringLiteral,
//                  AST::IdentifierLiteral>("foo(1.0, \"Hi\", bar)", "foo",
//                  {1.0},
//                                          {"\"Hi\""}, {"bar"});
//
//    parseFunction<AST::FloatLiteral, AST::StringLiteral,
//    AST::IdentifierLiteral,
//                  AST::BoolLiteral>("foo(1.0, \"Hi\", bar, true)", "foo",
//                  {1.0},
//                                    {"\"Hi\""}, {"bar"}, {true});
//}

TEST_F(FunctionDefTest, FunctionDeclaration) {
    parseFunctionDef("def foo() {}", "foo", TypeHelper::Type::Int, {});
}

TEST_F(FunctionDefTest, FunctionDeclarationWithArgs) {
    parseFunctionDef("def foo(x) {}", "foo", TypeHelper::Type::Int,
                     {
                         std::make_shared<AST::FormalParameter>("x"),
                     });

    parseFunctionDef("def foo(x, y) {}", "foo", TypeHelper::Type::Float,
                     {
                         std::make_shared<AST::FormalParameter>("x"),
                         std::make_shared<AST::FormalParameter>("y"),
                     });

    parseFunctionDef("def foo(x, y, _abc, sure) {}", "foo",
                     TypeHelper::Type::Bool,
                     {
                         std::make_shared<AST::FormalParameter>("x"),
                         std::make_shared<AST::FormalParameter>("y"),
                         std::make_shared<AST::FormalParameter>("_abc"),
                         std::make_shared<AST::FormalParameter>("sure"),
                     });
}

TEST_F(BlockTest, SimpleBlock) {
    statements_t statements;
    auto block = std::make_shared<AST::Block>(statements);
    parseBlock("{}", block);

    statements.push_back(std::make_shared<AST::Declaration>(
        "x", std::make_shared<IntLiteral>(42)));
    parseBlock("{\nvar x = 42;\n}", std::make_shared<AST::Block>(statements));
}

TEST_F(VariableDeclarationTest, SimpleDeclaration) {
    parseDeclarationDetails<AST::BoolLiteral>(
        "var x = true;", std::make_shared<AST::BoolLiteral>(true));
    parseDeclarationDetails<AST::BoolLiteral>(
        "var x = false;", std::make_shared<AST::BoolLiteral>(false));
    parseDeclarationDetails<AST::IntLiteral>(
        "var x = 42;", std::make_shared<AST::IntLiteral>(42));
    parseDeclarationDetails<AST::FloatLiteral>(
        "var x = 1.23;", std::make_shared<AST::FloatLiteral>(1.23f));
    parseDeclarationDetails<AST::StringLiteral>(
        "var x = \"abc\";", std::make_shared<AST::StringLiteral>("abc"));
    parseDeclarationDetails<AST::IdentifierLiteral>(
        "var x = y;", std::make_shared<AST::IdentifierLiteral>("y"));
}

TEST_F(VariableDeclarationTest, BoolDeclarationErrors) {
    std::vector<std::string> erroneous_program = {
        "var x : true;",
        "var x:  = true;",
        "var 123 bool  = true;",
        "var x: bool = true;",
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
    parseAssignmentDetails<AST::IntLiteral>(
        "x = 123;", std::make_shared<AST::IntLiteral>(123));
    parseAssignmentDetails<AST::FloatLiteral>(
        "x = 123.456;", std::make_shared<AST::FloatLiteral>(123.456f));
    parseAssignmentDetails<AST::BoolLiteral>(
        "x = true;", std::make_shared<AST::BoolLiteral>(true));
    parseAssignmentDetails<AST::StringLiteral>(
        "x = \"123\";", std::make_shared<AST::StringLiteral>("123"));
    parseAssignmentDetails<AST::IdentifierLiteral>(
        "x = y;", std::make_shared<AST::IdentifierLiteral>("y"));
}
