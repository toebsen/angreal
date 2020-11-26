//
// Created by bichlmaier on 05.02.2020.
//

#ifndef ANGREAL_TOKEN_H
#define ANGREAL_TOKEN_H

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <magic_enum.hpp>

#include "common.h"
#include "state.h"

namespace angreal {

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
        Dot,
        Colon,
        SemiColon,
        Equal,
        Exclamation,
        RelationalOp,
        Boolean,
        IfStatement,
        PrintStatement,
        DefStatement,
        ClassStatement,
        VarStatement,
        ReturnStatement,
        WhileStatement,
        ForStatement,
        AndStatement,
        OrStatement,
        NotStatement,
        SelfStatement,
        NewLine,
        EndOfProgram,
        Error,
    };

    static string_t type2str(Type t) {
        return string_t(magic_enum::enum_name(t));
    };

    struct Position {
        int line;
    };

    Token(const string_t& value, angreal::State finalState, const Position& pos)
        : value_(value), type_(infer(value, finalState)), position_(pos) {}

    Token(string_t value, const Type& type)
        : value_(std::move(value)), type_(type) {}

    [[nodiscard]] inline Type type() const { return type_; };

    [[nodiscard]] inline string_t value() const { return value_; };

    [[nodiscard]] Position position() const { return position_; };

    friend std::ostream& operator<<(std::ostream& os,
                                    const angreal::Token& tok) {
        os << "Token(" << magic_enum::enum_name(tok.type()) << ", '"
           << tok.value() << "'";
        os << " Line: " << tok.position().line << ")";
        return os;
    }

    Token::Type infer(const string_t& value, const angreal::State& finalSate) {
        switch (finalSate) {
            case angreal::State::Digit:
                return Type::Integer;
            case angreal::State::DigitPeriod:
                return Type::Float;
            case angreal::State::AddSub:
                return Type::AdditiveOp;
            case angreal::State::Asterisk:
                return Type::MulOp;
            case angreal::State::Slash:
                return Type::DivOp;
            case angreal::State::EndComment:
                return Type::Comment;
            case angreal::State::Identifier:
                return ReservedTokenFrom(value);
            case angreal::State::EndString:
                return Type::String;
            case angreal::State::Equals:
                return Type::Equal;
            case angreal::State::Exclamation:
                return Type::Exclamation;
            case angreal::State::Punctuation:
                return punctuation_.at(value);
            case angreal::State::SingleRelational:
            case angreal::State::CombinedRelational:
                return Type::RelationalOp;
            case angreal::State::NewLine:
                return Type::NewLine;
            case angreal::State::EndOfProgram:
                return Type::EndOfProgram;
            case State::Period:
                return Type::Dot;
            case State::StartComment:
            case State::StartString:
            case State::WhiteSpace:
            case State::Invalid:
            case State::Initial:
                return Type::Error;
        }
        return Type::Error;
    };

   private:
    [[nodiscard]] Token::Type ReservedTokenFrom(const string_t& value) const {
        if (reserved_.contains(value)) {
            return reserved_.at(value);
        }
        return Type::Identifier;
    }

    const std::map<string_t, Token::Type> reserved_ {
        {"true", Type::Boolean},
        {"false", Type::Boolean},
        {"if", Type::IfStatement},
        {"def", Type::DefStatement},
        {"var", Type::VarStatement},
        {"return", Type::ReturnStatement},
        {"while", Type::WhileStatement},
        {"for", Type::ForStatement},
        {"and", Type::AndStatement},
        {"or", Type::OrStatement},
        {"not", Type::NotStatement},
        {"int", Type::IntIdentifier},
        {"bool", Type::BoolIdentifier},
        {"float", Type::FloatIdentifier},
        {"string", Type::StringIdentifier},
        {"print", Type::PrintStatement},
        {"class", Type::ClassStatement},
        {"self", Type::SelfStatement},
    };

    const std::map<string_t, Token::Type> punctuation_ {
        {"{", Type::LeftCurlyBracket},
        {"}", Type::RightCurlyBracket},
        {"(", Type::LeftBracket},
        {")", Type::RightBracket},
        {",", Type::Comma},
        {":", Type::Colon},
        {";", Type::SemiColon},
    };

    Position position_ {-1};
    Type type_;
    string_t value_;
};

}  // namespace angreal

#endif  // ANGREAL_TOKEN_H
