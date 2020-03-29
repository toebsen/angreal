//
// Created by bichlmaier on 10.02.2020.
//

#include "type_system.h"
#include "ast.h"

namespace tb_lang::parser {

Token::Type TypeSystem::mapLiteralTypeToToken(TypeSystem::Type expected)
{
  switch (expected) {
  case Type::Bool:
    return Token::Type::Boolean;
  case Type::Int:
    return Token::Type::Integer;
  case Type::Float:
    return Token::Type::Float;
  case Type::String:
    return Token::Type::String;
  }
  return Token::Type::Error;
}

TypeSystem::Type TypeSystem::mapTokenToLiteralType(Token::Type expected)
{
  switch (expected) {
  case Token::Type::Boolean:
    return Type::Bool;
  case Token::Type::Integer:
    return Type::Int;
  case Token::Type::Float:
    return Type::Float;
  case Token::Type::String:
    return Type::String;
  }
  return Type::Unknown;
}

TypeSystem::Type TypeSystem::mapTokenToDeclarationType(Token::Type token)
{
  switch (token) {
  case Token::Type::BoolIdentifier:
    return Type::Bool;
  case Token::Type::IntIdentifier:
    return Type::Int;
  case Token::Type::FloatIdentifier:
    return Type::Float;
  case Token::Type::StringIdentifier:
    return Type::String;
  }
  return Type::Unknown;
}

std::shared_ptr<AST::Expression>
  TypeSystem::mapTypeToLiteral(TypeSystem::Type expected,
    const std::string &value)
{
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
};
}// namespace tb_lang::parser