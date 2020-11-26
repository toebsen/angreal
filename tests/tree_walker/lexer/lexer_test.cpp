//
// Created by bichlmaier on 05.02.2020.
//
#include "lexer/lexer.h"

#include <string>

#include <gtest/gtest.h>

#include "token.h"

using namespace angreal;
using namespace angreal::lex;

class LexerTest : public ::testing::Test {
   protected:
    typedef std::vector<Token::Type> tokens_t;

    void lexSequence(const std::vector<std::string>& vals,
                     const tokens_t& expected) {
        for (size_t i = 0; i < vals.size(); ++i) {
            auto v = vals[i];
            auto tok = expected[i];

            auto t = lexer.lex(v);
            auto identifier_ = t[0];
            EXPECT_EQ(tok, identifier_.type());
            EXPECT_EQ(v, identifier_.value());
        }
    }

    void lexSequence(const std::vector<std::string>& vals,
                     const Token::Type& expected) {
        const tokens_t tokens(vals.size(), expected);
        lexSequence(vals, tokens);
    }

    void lexProgram(const std::string& program,
                    const std::vector<Token>& expected) {
        auto prog_tokens = lexer.lex(program);
        ASSERT_EQ(prog_tokens.size(), expected.size());
        for (size_t i = 0; i < prog_tokens.size(); ++i) {
            EXPECT_EQ(prog_tokens[i].type(), expected[i].type());
            EXPECT_EQ(prog_tokens[i].value(), expected[i].value());
        }
    }

    Lexer lexer;
};

TEST_F(LexerTest, EmptyProgram) {
    auto t = lexer.lex("");
    EXPECT_EQ(1, t.size());
    EXPECT_EQ(Token::Type::EndOfProgram, t[0].type());
    // with Whitespace
    t = lexer.lex(" \n\t");
    EXPECT_EQ(Token::Type::NewLine, t[0].type());
    EXPECT_EQ(Token::Type::EndOfProgram, t[1].type());
}

TEST_F(LexerTest, ProgramWithInt) {
    lexSequence({"1", "1235", "56789", "1234"}, Token::Type::Integer);
}

TEST_F(LexerTest, ProgramWithFloat) {
    lexSequence({"1.2", "1234.56789", "56789.1234", ".09", ".09"},
                Token::Type::Float);
}

TEST_F(LexerTest, AdditiveOperations) {
    lexSequence({"+", "-"}, Token::Type::AdditiveOp);
}

TEST_F(LexerTest, SimpleAddition) {
    auto t = lexer.lex("1+2");
    ASSERT_EQ(4, t.size());

    auto _1 = t[0];
    auto _2 = t[1];
    auto _3 = t[2];

    EXPECT_EQ(Token::Type::Integer, _1.type());
    EXPECT_EQ(Token::Type::AdditiveOp, _2.type());
    EXPECT_EQ(Token::Type::Integer, _3.type());
}

TEST_F(LexerTest, AsteriksMul) {
    auto t = lexer.lex("*");
    auto add_ = t[0];
    EXPECT_EQ(Token::Type::MulOp, add_.type());
    EXPECT_EQ("*", add_.value());
}

TEST_F(LexerTest, SlashDivisor) {
    auto t = lexer.lex("a/b");
    auto sub_ = t[1];
    EXPECT_EQ(Token::Type::DivOp, sub_.type());
    EXPECT_EQ("/", sub_.value());
}

TEST_F(LexerTest, HashtagComment) {
    auto t = lexer.lex("#abc 1\t23\n");
    auto comment_ = t[0];
    EXPECT_EQ(Token::Type::Comment, comment_.type());
    EXPECT_EQ("#abc 1\t23\\n", comment_.value());
}

TEST_F(LexerTest, Identifier) {
    lexSequence({"_abC", "xyZ", "x_Y_z"}, Token::Type::Identifier);
}

TEST_F(LexerTest, BooleanIdentifier) {
    lexSequence({"true", "false"}, Token::Type::Boolean);
}

TEST_F(LexerTest, BuiltInIdentifier) {
    lexSequence({"if", "def", "var", "return", "while", "for", "print", "and",
                 "or", "not", "int", "float", "bool", "string", "class"},
                {Token::Type::IfStatement, Token::Type::DefStatement,
                 Token::Type::VarStatement, Token::Type::ReturnStatement,
                 Token::Type::WhileStatement, Token::Type::ForStatement,
                 Token::Type::PrintStatement, Token::Type::AndStatement,
                 Token::Type::OrStatement, Token::Type::NotStatement,
                 Token::Type::IntIdentifier, Token::Type::FloatIdentifier,
                 Token::Type::BoolIdentifier, Token::Type::StringIdentifier,
                 Token::Type::ClassStatement});
}

TEST_F(LexerTest, Punctuation) {
    std::vector<std::string> vals {"{", "}", "(", ")", ",", ":", ";", "."};
    std::vector<Token::Type> tokens = {
        Token::Type::LeftCurlyBracket,
        Token::Type::RightCurlyBracket,
        Token::Type::LeftBracket,
        Token::Type::RightBracket,
        Token::Type::Comma,
        Token::Type::Colon,
        Token::Type::SemiColon,
        Token::Type::Dot,
    };
    lexSequence(vals, tokens);
}

TEST_F(LexerTest, Equals) {
    auto t = lexer.lex("=");
    auto identifier_ = t[0];
    EXPECT_EQ(Token::Type::Equal, identifier_.type());
    EXPECT_EQ("=", identifier_.value());
}

TEST_F(LexerTest, Exclamation) {
    auto t = lexer.lex("!");
    auto identifier_ = t[0];
    EXPECT_EQ(Token::Type::Exclamation, identifier_.type());
    EXPECT_EQ("!", identifier_.value());
}

TEST_F(LexerTest, Relational) {
    std::vector<std::string> vals {"<", ">", "<=", ">=", "==", "!="};
    lexSequence(vals, Token::Type::RelationalOp);
}
TEST_F(LexerTest, Dot) {
    auto t = lexer.lex("a.b");
    std::vector<Token::Type> expected {
        Token::Type::Identifier, Token::Type::Dot, Token::Type::Identifier,
        Token::Type::EndOfProgram};

    ASSERT_EQ(t.size(), expected.size());
    for (int i = 0; i < t.size(); ++i) {
        EXPECT_EQ(t[i].type(), expected[i]);
    }
}

TEST_F(LexerTest, AcceptenceTest1) {
    std::string test = "var x = 1 + 2;";

    std::vector<Token> expected = {
        {"var", Token::Type::VarStatement},
        {"x", Token::Type::Identifier},
        {"=", Token::Type::Equal},
        {"1", Token::Type::Integer},
        {"+", Token::Type::AdditiveOp},
        {"2", Token::Type::Integer},
        {";", Token::Type::SemiColon},
        {"<EOP>", Token::Type::EndOfProgram},
    };

    lexProgram(test, expected);
}

TEST_F(LexerTest, AcceptenceTest2) {
    const char* prog = R"(# Calculate Fibonacci sequence
    def fib(n) {
            if(n == 0){
                return 0;
            }
            else{
        # so will this one
                if(n == 1){
                    return 1;
                }
                else{
                    return fib(n-1) + fib(n-2);
                }
            }
    })";

    std::vector<Token> expected = {
        {"# Calculate Fibonacci sequence\\n", Token::Type::Comment},
        {"def", Token::Type::DefStatement},
        {"fib", Token::Type::Identifier},
        {"(", Token::Type::LeftBracket},
        {"n", Token::Type::Identifier},
        {")", Token::Type::RightBracket},
        {"{", Token::Type::LeftCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"if", Token::Type::IfStatement},
        {"(", Token::Type::LeftBracket},
        {"n", Token::Type::Identifier},
        {"==", Token::Type::RelationalOp},
        {"0", Token::Type::Integer},
        {")", Token::Type::RightBracket},
        {"{", Token::Type::LeftCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"return", Token::Type::ReturnStatement},
        {"0", Token::Type::Integer},
        {";", Token::Type::SemiColon},
        {R"(\n)", Token::Type::NewLine},
        {"}", Token::Type::RightCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"else", Token::Type::Identifier},
        {"{", Token::Type::LeftCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"# so will this one\\n", Token::Type::Comment},
        {"if", Token::Type::IfStatement},
        {"(", Token::Type::LeftBracket},
        {"n", Token::Type::Identifier},
        {"==", Token::Type::RelationalOp},
        {"1", Token::Type::Integer},
        {")", Token::Type::RightBracket},
        {"{", Token::Type::LeftCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"return", Token::Type::ReturnStatement},
        {"1", Token::Type::Integer},
        {";", Token::Type::SemiColon},
        {R"(\n)", Token::Type::NewLine},
        {"}", Token::Type::RightCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"else", Token::Type::Identifier},
        {"{", Token::Type::LeftCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"return", Token::Type::ReturnStatement},
        {"fib", Token::Type::Identifier},
        {"(", Token::Type::LeftBracket},
        {"n", Token::Type::Identifier},
        {"-", Token::Type::AdditiveOp},
        {"1", Token::Type::Integer},
        {")", Token::Type::RightBracket},
        {"+", Token::Type::AdditiveOp},
        {"fib", Token::Type::Identifier},
        {"(", Token::Type::LeftBracket},
        {"n", Token::Type::Identifier},
        {"-", Token::Type::AdditiveOp},
        {"2", Token::Type::Integer},
        {")", Token::Type::RightBracket},
        {";", Token::Type::SemiColon},
        {R"(\n)", Token::Type::NewLine},
        {"}", Token::Type::RightCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"}", Token::Type::RightCurlyBracket},
        {R"(\n)", Token::Type::NewLine},
        {"}", Token::Type::RightCurlyBracket},
        {"<EOP>", Token::Type::EndOfProgram},
    };

    lexProgram(prog, expected);
}