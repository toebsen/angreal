//
// Created by bichlmaier on 10.02.2020.
//

#include "type_helper.h"

#include "ast.h"

namespace tb_lang::parser {

TypeHelper::Type TypeHelper::mapTokenToLiteralType(Token::Type expected) {
    switch (expected) {
        case Token::Type::Boolean:
            return Type::Bool;
        case Token::Type::Integer:
            return Type::Int;
        case Token::Type::Float:
            return Type::Float;
        case Token::Type::String:
            return Type::String;
        default:
            return Type::Unknown;
    }
}

std::shared_ptr<AST::Expression> TypeHelper::mapTypeToLiteral(
    TypeHelper::Type expected, const std::string& value) {
    switch (expected) {
        case Type::Bool:
            return std::make_shared<AST::BoolLiteral>(value);
        case Type::Int:
            return std::make_shared<AST::IntLiteral>(value);
        case Type::Float:
            return std::make_shared<AST::FloatLiteral>(value);
        case Type::String:
            return std::make_shared<AST::StringLiteral>(value);
        case Type::Unknown:
            return nullptr;
    }
    return nullptr;
}

}  // namespace tb_lang::parser