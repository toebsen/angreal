//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_AST_H
#define TBLANG_AST_H

#include <memory>
#include <vector>

#include <ast_interfaces.h>
#include <visitor.h>

#include "type_system.h"

namespace tb_lang::parser::AST {

class Program : public Node {
   public:
    explicit Program(nodes_t nodes) : statements(nodes){};

    void accept(Visitor* visitor) override;

    virtual ~Program(){};
    nodes_t statements;
};

class Block : public Statement {
   public:
    explicit Block(statements_t statements) : statements(statements){};

    virtual void accept(Visitor* visitor) override;

    virtual ~Block() = default;

    statements_t statements;
};

class Declaration : public Statement {
   public:
    Declaration(const TypeSystem::Type& type, const std::string& identifier,
                expression_t expression)
        : type(type), identifier(identifier), expression(expression){};

    virtual void accept(Visitor* visitor) override;

    virtual ~Declaration() = default;

    const TypeSystem::Type type;
    expression_t expression;
    const std::string identifier;
};

class Assignment : public Statement {
   public:
    Assignment(const std::string& identifier, expression_t expression)
        : identifier(identifier), expression(expression){};

    virtual void accept(Visitor* visitor) override;

    virtual ~Assignment() = default;

    expression_t expression;
    const std::string identifier;
};

class IdentifierLiteral : public Expression {
   public:
    IdentifierLiteral(std::string identifier) : name(std::move(identifier)){};

    virtual void accept(Visitor* visitor) override;

    bool operator==(const IdentifierLiteral& rhs) const {
        return name == rhs.name;
    }

    const std::string name;
};

template <typename ValueType>
class ValueLiteral : public Expression {
   public:
    explicit ValueLiteral(const ValueType& value) : value(value){};

    typedef ValueType value_t;

    static TypeSystem::Type type() { return TypeSystem::Type::Unknown; };

    bool operator==(const ValueLiteral& rhs) const {
        return type() == rhs.type() && value == rhs.value;
    }

    const ValueType value;
};

class BoolLiteral : public ValueLiteral<bool> {
   public:
    BoolLiteral(const std::string& value);

    BoolLiteral(bool value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Bool; };

    virtual void accept(Visitor* visitor) override { visitor->visit(this); };
};

class IntLiteral : public ValueLiteral<int> {
   public:
    IntLiteral(const std::string& value);

    IntLiteral(int value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Int; };

    virtual void accept(Visitor* visitor) override { visitor->visit(this); };
};

class FloatLiteral : public ValueLiteral<float> {
   public:
    FloatLiteral(const std::string& value);

    FloatLiteral(float value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Float; };

    virtual void accept(Visitor* visitor) override { visitor->visit(this); };
};

class StringLiteral : public ValueLiteral<std::string> {
   public:
    StringLiteral(const std::string& value);

    static TypeSystem::Type type() { return TypeSystem::Type::String; };

    virtual void accept(Visitor* visitor) override { visitor->visit(this); };
};

class UnaryOperation : public Expression {
   public:
    enum class OpType {
        Unknown,
        Add,
        Sub,
        Not,
    };

    static OpType inferType(const std::string& value) {
        if (value == "+") return OpType::Add;
        if (value == "-") return OpType::Sub;
        if (value == "!") return OpType::Not;

        return OpType::Unknown;
    }

    UnaryOperation(const std::string& opType, expression_t expression)
        : UnaryOperation(inferType(opType), expression){};

    UnaryOperation(OpType opType, expression_t expression)
        : type(opType), expression(expression){};

    void accept(Visitor* visitor) override;

    OpType type;
    expression_t expression;
};

class BinaryOperation : public Expression {
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

    static OpType inferType(const std::string& value) {
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

    BinaryOperation(const std::string& opType, expression_t lhs,
                    expression_t rhs)
        : BinaryOperation(inferType(opType), lhs, rhs){};

    BinaryOperation(OpType opType, expression_t lhs, expression_t rhs)
        : type(opType), lhs(lhs), rhs(rhs){};

    void accept(Visitor* visitor) override;

    OpType type;
    expression_t lhs;
    expression_t rhs;
};

class FunctionCall : public Expression {
   public:
    FunctionCall(const std::string& identifier, expressions_t args)
        : identifier(identifier), args(args){};

    void accept(Visitor* visitor) override;

    std::string identifier;
    expressions_t args;
};

class FormalParameter : public Expression {
   public:
    FormalParameter(TypeSystem::Type type, const std::string& identifier)
        : type(type), identifier(identifier) {}

    bool operator==(const FormalParameter& other) const {
        return type == other.type && identifier == other.identifier;
    }

    void accept(Visitor* visitor) override;

    TypeSystem::Type type;
    std::string identifier;
};

typedef std::vector<std::shared_ptr<FormalParameter>> formal_parameters;
class FunctionDeclaration : public Statement {
   public:
    FunctionDeclaration(const TypeSystem::Type& type,
                        const std::string& identifier,
                        const formal_parameters& parameters,
                        statements_t statements)
        : type(type),
          identifier(identifier),
          parameters(parameters),
          statements(statements){};

    void accept(Visitor* visitor) override;

    std::string identifier;
    const TypeSystem::Type type;
    formal_parameters parameters;
    statements_t statements;
};
}  // namespace tb_lang::parser::AST

#endif  // TBLANG_AST_H
