//
// Created by toebs on 14.05.2020.
//

#ifndef ANGREAL_TESTS_PARSER_TEST_FIXTURES_H_
#define ANGREAL_TESTS_PARSER_TEST_FIXTURES_H_

#include <string>

#include <gtest/gtest.h>

#include "lexer/lexer.h"
#include "parser/ast.h"
#include "parser/parser.h"
#include "token.h"

using namespace angreal;
using namespace angreal::lex;
using namespace angreal::parser;

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

    std::string defaultIdentifier {"x"};
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

class ClassDeclarationTest : public VariableDeclarationTest {};

class VariableAssignmentTest : public ParserTest {
   protected:
    template <typename Expression>
    void parseAssignmentDetails(const std::string& program,
                                expression_t expression) {
        auto ast = lexAndParseProgram(program);
        ASSERT_EQ(1, ast->statements.size());
        auto stmt = std::dynamic_pointer_cast<AST::ExpressionStatement>(
            ast->statements[0]);
        auto assignment =
            std::dynamic_pointer_cast<Assignment>(stmt->expression);
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
    template <typename T>
    auto parseSingleExpressionAs(const std::string& program) {
        auto expr = lexAndParseExpression(program);
        ASSERT_NE(nullptr, expr);
        EXPECT_NE(nullptr, std::dynamic_pointer_cast<T>(expr));
    }

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

#endif  // ANGREAL_TESTS_PARSER_TEST_FIXTURES_H_
