//
// Created by bichlmaier on 05.02.2020.
//

#include "test_fixtures.h"

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

TEST_F(ExpressionTest, Self) { parseSingleExpressionAs<AST::Self>("self"); }

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

TEST_F(ExpressionTest, Super) {
    parseSingleExpressionAs<AST::Super>("super.test");
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
        lexAndParseProgram(t);
        EXPECT_EQ(true, error_handler_->HasError());
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
        lexAndParseProgram(t);
        EXPECT_EQ(true, error_handler_->HasError());
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
        lexAndParseProgram(t);
        EXPECT_EQ(true, error_handler_->HasError());
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
        lexAndParseProgram(t);
        EXPECT_EQ(true, error_handler_->HasError());
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

template <typename Expected, typename Actual>
void ASSERT_EQ_SHARED_PTR(std::shared_ptr<Expected> val1,
                          std::shared_ptr<Actual> val2) {
    auto _cast = std::dynamic_pointer_cast<Expected>(val2);
    ASSERT_NE(nullptr, _cast);
    ASSERT_EQ(*val1, *_cast);
}

TEST_F(BlockTest, IfBlock) {
    auto condition = std::make_shared<AST::BoolLiteral>(true);

    auto prog = lexAndParseProgram(R"(if(true){print(42);}})");
    ASSERT_FALSE(prog->statements.empty());

    auto if_statement =
        std::dynamic_pointer_cast<AST::IfStatement>(prog->statements[0]);

    ASSERT_EQ_SHARED_PTR(condition, if_statement->condition);
    ASSERT_EQ(1, if_statement->if_branch->statements.size());
    ASSERT_EQ(nullptr, if_statement->else_branch);
}

TEST_F(BlockTest, IfElseBlock) {
    auto condition = std::make_shared<AST::BoolLiteral>(false);

    auto prog =
        lexAndParseProgram(R"(if(false){print(1);}else{print(1);print(2);})");
    ASSERT_FALSE(prog->statements.empty());

    auto if_statement =
        std::dynamic_pointer_cast<AST::IfStatement>(prog->statements[0]);

    ASSERT_EQ_SHARED_PTR(condition, if_statement->condition);
    ASSERT_EQ(1, if_statement->if_branch->statements.size());
    ASSERT_EQ(2, if_statement->else_branch->statements.size());
}

TEST_F(BlockTest, While) {
    auto condition = std::make_shared<AST::BoolLiteral>(true);

    auto prog = lexAndParseProgram(R"(while(true){print(42);}})");
    ASSERT_FALSE(prog->statements.empty());

    auto while_statement =
        std::dynamic_pointer_cast<AST::WhileStatement>(prog->statements[0]);

    ASSERT_EQ_SHARED_PTR(condition, while_statement->condition);
    ASSERT_EQ(1, while_statement->block->statements.size());
}

TEST_F(ClassDeclarationTest, EmpyClassDeclaration) {
    auto prog = lexAndParseProgram(R"(
        class EmptyClass{};
    )");
    ASSERT_FALSE(prog->statements.empty());

    auto class_declaration =
        std::dynamic_pointer_cast<AST::ClassDeclaration>(prog->statements[0]);

    ASSERT_EQ("EmptyClass", class_declaration->identifier);
    ASSERT_EQ(0, class_declaration->methods.size());
}

TEST_F(ClassDeclarationTest, EmpyClassWithSuperDeclaration) {
    auto prog = lexAndParseProgram(R"(
        class EmptyClass(OtherClass){};
    )");
    ASSERT_FALSE(prog->statements.empty());

    auto class_declaration =
        std::dynamic_pointer_cast<AST::ClassDeclaration>(prog->statements[0]);

    ASSERT_EQ("EmptyClass", class_declaration->identifier);
    ASSERT_EQ("OtherClass", std::dynamic_pointer_cast<IdentifierLiteral>(
                                class_declaration->superclass.value())
                                ->name);
    ASSERT_EQ(0, class_declaration->methods.size());
}

TEST_F(ClassDeclarationTest, EmpyClassWithStringSuperDeclaration) {
    lexAndParseProgram(R"(
        class EmptyClass("OtherClass"){};
    )");

    EXPECT_EQ(true, error_handler_->HasError());
}

TEST_F(ClassDeclarationTest, ClassDeclaration) {
    auto prog = lexAndParseProgram(R"(
        class MyClass
        {
            def myMethod()
            {
            }
        }
    )");
    ASSERT_FALSE(prog->statements.empty());

    auto class_declaration =
        std::dynamic_pointer_cast<AST::ClassDeclaration>(prog->statements[0]);

    ASSERT_EQ("MyClass", class_declaration->identifier);
    ASSERT_EQ(1, class_declaration->methods.size());
    ASSERT_EQ("myMethod", class_declaration->methods[0]->identifier);
}

TEST_F(ClassDeclarationTest, ClassDeclarationWithVarDecl) {
    lexAndParseProgram(R"(
        class MyClass
        {
            var x = 10;
        }
    )");

    EXPECT_EQ(true, error_handler_->HasError());
}

TEST_F(ClassDeclarationTest, GetExprTest) {
    auto prog = lexAndParseProgram(R"(
        x.y
    )");
    auto expr_stmt =
        std::dynamic_pointer_cast<ExpressionStatement>(prog->statements[0]);
    auto get_expr = std::dynamic_pointer_cast<Get>(expr_stmt->expression);

    ASSERT_NE(nullptr, get_expr);
    EXPECT_EQ("x",
              std::dynamic_pointer_cast<IdentifierLiteral>(get_expr->expression)
                  ->name);
    EXPECT_EQ("y", get_expr->identifier);
}

TEST_F(ClassDeclarationTest, SetExprTest) {
    auto prog = lexAndParseProgram(R"(
        x.y = 123
    )");
    auto expr_stmt =
        std::dynamic_pointer_cast<ExpressionStatement>(prog->statements[0]);
    auto set_expr = std::dynamic_pointer_cast<Set>(expr_stmt->expression);
    auto ident =
        std::dynamic_pointer_cast<IdentifierLiteral>(set_expr->expression);

    ASSERT_NE(nullptr, set_expr);
    EXPECT_EQ("x", ident->name);
    EXPECT_EQ("y", set_expr->identifier);
    EXPECT_EQ(123,
              std::dynamic_pointer_cast<IntLiteral>(set_expr->value)->value);
}