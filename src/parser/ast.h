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

class Program : public Node, public std::enable_shared_from_this<Program> {
   public:
    explicit Program(const statements_t& statements) : statements(statements){};

    void accept(const visitor_t& visitor) override;

    virtual ~Program(){};
    statements_t statements;
};

class Block : public Statement, public std::enable_shared_from_this<Block> {
   public:
    explicit Block(const statements_t& statements) : statements(statements){};

    void accept(const visitor_t& visitor) override;

    virtual ~Block() = default;

    statements_t statements;
};

using block_t = std::shared_ptr<Block>;

class ExpressionStatement
    : public Statement,
      public std::enable_shared_from_this<ExpressionStatement> {
   public:
    explicit ExpressionStatement(const expression_t& expression)
        : expression(expression){};

    void accept(const visitor_t& visitor) override;

    virtual ~ExpressionStatement() = default;

    expression_t expression;
};

class Declaration : public Statement,
                    public std::enable_shared_from_this<Declaration> {
   public:
    Declaration(const TypeSystem::Type& type, const std::string& identifier,
                const expression_t& expression)
        : type(type), identifier(identifier), expression(expression){};

    void accept(const visitor_t& visitor) override;

    virtual ~Declaration() = default;

    const TypeSystem::Type type;
    expression_t expression;
    const std::string identifier;
};

class Assignment : public Statement,
                   public std::enable_shared_from_this<Assignment> {
   public:
    Assignment(const std::string& identifier, const expression_t& expression)
        : identifier(identifier), expression(expression){};

    void accept(const visitor_t& visitor) override;

    virtual ~Assignment() = default;

    expression_t expression;
    const std::string identifier;
};

class Return : public Statement, public std::enable_shared_from_this<Return> {
   public:
    Return(const expression_t& expression) : expression(expression){};

    void accept(const visitor_t& visitor) override;

    virtual ~Return() = default;

    expression_t expression;
};

class Print : public Statement, public std::enable_shared_from_this<Print> {
   public:
    Print(const expressions_t& expressions) : expressions(expressions){};

    void accept(const visitor_t& visitor) override;

    virtual ~Print() = default;

    expressions_t expressions;
};

class IdentifierLiteral
    : public Expression,
      public std::enable_shared_from_this<IdentifierLiteral> {
   public:
    IdentifierLiteral(std::string identifier) : name(std::move(identifier)){};

    void accept(const visitor_t& visitor) override;

    bool operator==(const IdentifierLiteral& rhs) const {
        return name == rhs.name;
    }

    const std::string name;
};

template <typename ValueType>
class ValueLiteral : public Expression {
   public:
    explicit ValueLiteral(const ValueType& value) : value(value){};
    virtual ~ValueLiteral() = default;

    typedef ValueType value_t;

    static TypeSystem::Type type() { return TypeSystem::Type::Unknown; };

    bool operator==(const ValueLiteral& rhs) const {
        return type() == rhs.type() && value == rhs.value;
    }

    const ValueType value;
};

class BoolLiteral : public ValueLiteral<bool>,
                    public std::enable_shared_from_this<BoolLiteral> {
   public:
    BoolLiteral(const std::string& value);

    BoolLiteral(bool value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Bool; };

    void accept(const visitor_t& visitor) override;
};

class IntLiteral : public ValueLiteral<int>,
                   public std::enable_shared_from_this<IntLiteral> {
   public:
    IntLiteral(const std::string& value);

    IntLiteral(int value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Int; };

    void accept(const visitor_t& visitor) override;
};

class FloatLiteral : public ValueLiteral<float>,
                     public std::enable_shared_from_this<FloatLiteral> {
   public:
    FloatLiteral(const std::string& value);

    FloatLiteral(float value) : ValueLiteral(value){};

    static TypeSystem::Type type() { return TypeSystem::Type::Float; };

    void accept(const visitor_t& visitor) override;
};

class StringLiteral : public ValueLiteral<std::string>,
                      public std::enable_shared_from_this<StringLiteral> {
   public:
    StringLiteral(const std::string& value);

    static TypeSystem::Type type() { return TypeSystem::Type::String; };

    void accept(const visitor_t& visitor) override;
};

class UnaryOperation : public Expression,
                       public std::enable_shared_from_this<UnaryOperation> {
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

    UnaryOperation(const std::string& opType, const expression_t& expression)
        : UnaryOperation(inferType(opType), expression){};

    UnaryOperation(OpType opType, const expression_t& expression)
        : type(opType), expression(expression){};

    void accept(const visitor_t& visitor) override;

    OpType type;
    expression_t expression;
};

class BinaryOperation : public Expression,
                        public std::enable_shared_from_this<BinaryOperation> {
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
        GreaterEquals,
        LessEquals,
        Greater,
        Less,
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
        if (value == ">") return OpType::Greater;
        if (value == ">=") return OpType::GreaterEquals;
        if (value == "<") return OpType::Less;
        if (value == "<=") return OpType::LessEquals;

        return OpType::Unknown;
    }

    BinaryOperation(const std::string& opType, const expression_t& lhs,
                    const expression_t& rhs)
        : BinaryOperation(inferType(opType), lhs, rhs){};

    BinaryOperation(OpType opType, expression_t lhs, expression_t rhs)
        : type(opType), lhs(lhs), rhs(rhs){};

    void accept(const visitor_t& visitor) override;

    OpType type;
    expression_t lhs;
    expression_t rhs;
};

class FunctionCall : public Expression,
                     public std::enable_shared_from_this<FunctionCall> {
   public:
    FunctionCall(const std::string& identifier, const expressions_t& args)
        : identifier(identifier), args(args){};

    void accept(const visitor_t& visitor) override;

    std::string identifier;
    expressions_t args;
};

class FormalParameter : public Expression,
                        public std::enable_shared_from_this<FormalParameter> {
   public:
    FormalParameter(TypeSystem::Type type, const std::string& identifier)
        : type(type), identifier(identifier) {}

    bool operator==(const FormalParameter& other) const {
        return type == other.type && identifier == other.identifier;
    }

    void accept(const visitor_t& visitor) override;

    TypeSystem::Type type;
    std::string identifier;
};

using formal_parameters = std::vector<std::shared_ptr<FormalParameter>>;

class FunctionDeclaration
    : public Statement,
      public std::enable_shared_from_this<FunctionDeclaration> {
   public:
    FunctionDeclaration(const TypeSystem::Type& type,
                        const std::string& identifier,
                        const formal_parameters& parameters,
                        const statements_t& statements)
        : type(type),
          identifier(identifier),
          parameters(parameters),
          statements(statements){};

    void accept(const visitor_t& visitor) override;

    std::string identifier;
    const TypeSystem::Type type;
    formal_parameters parameters;
    statements_t statements;
};

class IfStatement : public Statement,
                    public std::enable_shared_from_this<IfStatement> {
   public:
    IfStatement(const expression_t& condition,  const block_t& block,
                const block_t& else_block)
        : condition(condition),
          block(block),
          else_block(else_block){};

    void accept(const visitor_t& visitor) override;

    expression_t condition;
    block_t block;
    block_t else_block;
};
}  // namespace tb_lang::parser::AST

#endif  // TBLANG_AST_H
