//
// Created by bichlmaier on 05.02.2020.
//

#ifndef TBLANG_TOKEN_H
#define TBLANG_TOKEN_H

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <magic_enum.hpp>

#include "state.h"

namespace tb_lang {

class Token {
   public:
    enum class Type {
        Integer = 1,
        Float,
        AdditiveOp,
        MulOp,
        DivOp,
        Comment,
        Identifier,
        IntIdentifier,
        BoolIdentifier,
        FloatIdentifier,
        StringIdentifier,
        String,
        LeftCurlyBracket,
        RightCurlyBracket,
        LeftBracket,
        RightBracket,
        Comma,
        Colon,
        SemiColon,
        Equal,
        Exclamation,
        RelationalOp,
        Boolean,
        IfStatement,
        PrintStatement,
        DefStatement,
        VarStatement,
        ReturnStatement,
        WhileStatement,
        ForStatement,
        AndStatement,
        OrStatement,
        NotStatement,
        NewLine,
        EndOfProgram,
        Error,
    };

    static std::string type2str(Type t) {
        return std::string(magic_enum::enum_name(t));
    };

    struct Position {
        int line;
    };

    Token(const std::string& value, tb_lang::State finalState,
          const Position& pos)
        : value_(value), type_(infer(value, finalState)), position_(pos) {}

    Token(std::string value, const Type& type)
        : value_(std::move(value)), type_(type) {}

    [[nodiscard]] inline Type type() const { return type_; };

    [[nodiscard]] inline std::string value() const { return value_; };

    [[nodiscard]] Position position() const { return position_; };

    friend std::ostream& operator<<(std::ostream& os,
                                    const tb_lang::Token& tok) {
        os << "Token(" << magic_enum::enum_name(tok.type()) << ", '"
           << tok.value() << "'";
        os << " Line: " << tok.position().line << ")";
        return os;
    }

    static Token::Type infer(const std::string& value,
                             const tb_lang::State& finalSate) {
        switch (finalSate) {
            case tb_lang::State::Digit:
                return Type::Integer;
            case tb_lang::State::DigitPeriod:
                return Type::Float;
            case tb_lang::State::AddSub:
                return Type::AdditiveOp;
            case tb_lang::State::Asterisk:
                return Type::MulOp;
            case tb_lang::State::Slash:
                return Type::DivOp;
            case tb_lang::State::EndComment:
                return Type::Comment;
            case tb_lang::State::Identifier:
                if (value == "true" || value == "false") {
                    return Type::Boolean;
                }
                if (value == "if") {
                    return Type::IfStatement;
                }
                if (value == "def") {
                    return Type::DefStatement;
                }
                if (value == "var") {
                    return Type::VarStatement;
                }
                if (value == "return") {
                    return Type::ReturnStatement;
                }
                if (value == "while") {
                    return Type::WhileStatement;
                }
                if (value == "for") {
                    return Type::ForStatement;
                }
                if (value == "and") {
                    return Type::AndStatement;
                }
                if (value == "or") {
                    return Type::OrStatement;
                }
                if (value == "not") {
                    return Type::NotStatement;
                }
                if (value == "int") {
                    return Type::IntIdentifier;
                }
                if (value == "bool") {
                    return Type::BoolIdentifier;
                }
                if (value == "float") {
                    return Type::FloatIdentifier;
                }
                if (value == "string") {
                    return Type::StringIdentifier;
                }
                if (value == "print") {
                    return Type::PrintStatement;
                }
                return Type::Identifier;
            case tb_lang::State::EndString:
                return Type::String;
            case tb_lang::State::Equals:
                return Type::Equal;
            case tb_lang::State::Exclamation:
                return Type::Exclamation;
            case tb_lang::State::Punctuation:
                if (value == "{") {
                    return Type::LeftCurlyBracket;
                }
                if (value == "}") {
                    return Type::RightCurlyBracket;
                }
                if (value == "(") {
                    return Type::LeftBracket;
                }
                if (value == ")") {
                    return Type::RightBracket;
                }
                if (value == ",") {
                    return Type::Comma;
                }
                if (value == ":") {
                    return Type::Colon;
                }
                if (value == ";") {
                    return Type::SemiColon;
                }
            case tb_lang::State::SingleRelational:
            case tb_lang::State::CombinedRelational:
                return Type::RelationalOp;
            case tb_lang::State::NewLine:
                return Type::NewLine;
            case tb_lang::State::EndOfProgram:
                return Type::EndOfProgram;
            case State::Invalid:
            case State::Initial:
            case State::Period:
            case State::StartComment:
            case State::StartString:
            case State::WhiteSpace:
                return Type::Error;
        }
        return Type::Error;
    };

   private:
    Position position_{-1};
    Type type_{Type::Error};
    std::string value_{""};
};

}  // namespace tb_lang

#endif  // TBLANG_TOKEN_H
