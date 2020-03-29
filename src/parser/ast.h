//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_AST_H
#define TBLANG_AST_H

#include <vector>
#include <memory>

#include <ast_interfaces.h>
#include <visitor.h>

#include "type_system.h"

namespace tb_lang::parser::AST {

class Program : public Node
{
public:
  explicit Program(nodes_t nodes) : statements(nodes){};

  virtual void accept(Visitor *visitor){};

  virtual ~Program(){};
  nodes_t statements;
};

class Block : public Statement
{
public:
  explicit Block(statements_t statements) : statements(statements){};

  virtual void accept(Visitor *visitor){};

  virtual ~Block() = default;

  statements_t statements;
};

class Declaration : public Statement
{
public:
  Declaration(const TypeSystem::Type &type, const std::string &identifier, expression_t expression)
    : type(type), identifier(identifier), expression(expression){};

  virtual void accept(Visitor *visitor){};

  virtual ~Declaration() = default;

  const TypeSystem::Type type;
  expression_t expression;
  const std::string identifier;
};

class Assignment : public Statement
{
public:
  Assignment(const std::string &identifier, expression_t expression)
    : identifier(identifier), expression(expression){};

  virtual void accept(Visitor *visitor){};

  virtual ~Assignment() = default;

  expression_t expression;
  const std::string identifier;
};

class IdentifierLiteral : public Expression
{
public:
  IdentifierLiteral(const std::string &name)
    : name(name){};

  virtual void accept(Visitor *visitor){};

  bool operator==(const IdentifierLiteral &rhs) const
  {
    return name == rhs.name;
  }

  const std::string name;
};

template<typename ValueType>
class ValueLiteral : public Expression
{
public:
  explicit ValueLiteral(const ValueType &value)
    : value(value){};

  typedef ValueType value_t;

  static TypeSystem::Type type() { return TypeSystem::Type::Unknown; };

  bool operator==(const ValueLiteral &rhs) const
  {
    return type() == rhs.type() && value == rhs.value;
  }

  virtual void accept(Visitor *visitor){};

  const ValueType value;
};

class BoolLiteral : public ValueLiteral<bool>
{
public:
  BoolLiteral(const std::string &value);

  BoolLiteral(bool value) : ValueLiteral(value){};

  static TypeSystem::Type type() { return TypeSystem::Type::Bool; };
};

class IntLiteral : public ValueLiteral<int>
{
public:
  IntLiteral(const std::string &value);

  IntLiteral(int value) : ValueLiteral(value){};

  static TypeSystem::Type type() { return TypeSystem::Type::Int; };
};

class FloatLiteral : public ValueLiteral<float>
{
public:
  FloatLiteral(const std::string &value);

  FloatLiteral(float value) : ValueLiteral(value){};

  static TypeSystem::Type type() { return TypeSystem::Type::Float; };
};

class StringLiteral : public ValueLiteral<std::string>
{
public:
  StringLiteral(const std::string &value);

  static TypeSystem::Type type() { return TypeSystem::Type::String; };
};

class UnaryOperation : public Expression
{
public:
  enum class OpType {
    Unknown,
    Add,
    Sub,
    Not,
  };

  static OpType inferType(const std::string &value)
  {
    if (value == "+") return OpType::Add;
    if (value == "-") return OpType::Sub;
    if (value == "!") return OpType::Not;

    return OpType::Unknown;
  }

  UnaryOperation(const std::string &opType, expression_t expression)
    : UnaryOperation(inferType(opType), expression){};

  UnaryOperation(OpType opType, expression_t expression)
    : type(opType), expression(expression){};

  virtual void accept(Visitor *visitor){};

  OpType type;
  expression_t expression;
};

class BinaryOperation : public Expression
{
public:
  enum class OpType {
    Unknown,
    Add,
    Sub,
    Mul,
    Divide,
    Or,
    And,
    Equals,
    NotEquals,
  };

  static OpType inferType(const std::string &value)
  {
    if (value == "+") return OpType::Add;
    if (value == "-") return OpType::Sub;
    if (value == "*") return OpType::Mul;
    if (value == "/") return OpType::Divide;
    if (value == "or") return OpType::Or;
    if (value == "and") return OpType::And;
    if (value == "==") return OpType::Equals;
    if (value == "!=") return OpType::NotEquals;

    return OpType::Unknown;
  }

  BinaryOperation(const std::string &opType, expression_t lhs, expression_t rhs)
    : BinaryOperation(inferType(opType), lhs, rhs){};

  BinaryOperation(OpType opType, expression_t lhs, expression_t rhs)
    : type(opType), lhs(lhs), rhs(rhs){};

  virtual void accept(Visitor *visitor){};

  OpType type;
  expression_t lhs;
  expression_t rhs;
};

class FunctionCall : public Expression
{
public:
  FunctionCall(const std::string &identifier, expressions_t args)
    : identifier(identifier), args(args){};

  virtual void accept(Visitor *visitor){};

  std::string identifier;
  expressions_t args;
};

class FormalParameter : public Expression
{
public:
  FormalParameter(TypeSystem::Type type, const std::string &identifier)
    : type(type), identifier(identifier) {}

  bool operator==(const FormalParameter &other) const
  {
    return type == other.type && identifier == other.identifier;
  }

  TypeSystem::Type type;
  std::string identifier;
};

typedef std::vector<std::shared_ptr<FormalParameter>> formal_parameters;
class FunctionDeclaration : public Statement
{
public:
  FunctionDeclaration(const TypeSystem::Type &type,
    const std::string &identifier,
    const formal_parameters &parameters,
    statements_t statements)
    : type(type), identifier(identifier), parameters(parameters), statements(statements){};

  virtual void accept(Visitor *visitor){};

  std::string identifier;
  const TypeSystem::Type type;
  formal_parameters parameters;
  statements_t statements;
};
}// namespace tb_lang::parser::AST

#endif//TBLANG_AST_H
