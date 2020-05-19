#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-special-member-functions"
#pragma ide diagnostic ignored "cppcoreguidelines-special-member-functions"
//
// Created by bichlmaier on 07.02.2020.
//

#ifndef ANGREAL_AST_H
#define ANGREAL_AST_H

#include <memory>
#include <utility>
#include <vector>

#include <ast_interfaces.h>
#include <visitor.h>

#include "type_helper.h"

namespace angreal::parser::AST {

class Program : public Node, public std::enable_shared_from_this<Program> {
   public:
    explicit Program(statements_t statements);

    void accept(const visitor_t& visitor) override;

    ~Program() override = default;

    statements_t statements;
};

class Block : public Statement, public std::enable_shared_from_this<Block> {
   public:
    explicit Block(statements_t statements);

    void accept(const visitor_t& visitor) override;

    ~Block() override = default;

    statements_t statements;
};

class ExpressionStatement
    : public Statement,
      public std::enable_shared_from_this<ExpressionStatement> {
   public:
    explicit ExpressionStatement(expression_t expression);

    void accept(const visitor_t& visitor) override;

    ~ExpressionStatement() override = default;

    expression_t expression;
};

class Declaration : public Statement,
                    public std::enable_shared_from_this<Declaration> {
   public:
    Declaration(std::string identifier, expression_t expression);

    void accept(const visitor_t& visitor) override;

    ~Declaration() override = default;

    expression_t expression;
    const std::string identifier;
};

class Assignment : public Expression,
                   public std::enable_shared_from_this<Assignment> {
   public:
    Assignment(std::string identifier, expression_t expression);

    void accept(const visitor_t& visitor) override;

    ~Assignment() override = default;

    expression_t expression;
    const std::string identifier;
};

class Return : public Statement, public std::enable_shared_from_this<Return> {
   public:
    explicit Return(expression_t expression);

    void accept(const visitor_t& visitor) override;

    ~Return() override = default;

    expression_t expression;
};

class Print : public Statement, public std::enable_shared_from_this<Print> {
   public:
    explicit Print(expressions_t expressions)
        : expressions(std::move(expressions)) {};

    void accept(const visitor_t& visitor) override;

    ~Print() override = default;

    expressions_t expressions;
};

class IdentifierLiteral
    : public Expression,
      public std::enable_shared_from_this<IdentifierLiteral> {
   public:
    explicit IdentifierLiteral(std::string identifier)
        : name(std::move(identifier)) {};

    void accept(const visitor_t& visitor) override;

    bool operator==(const IdentifierLiteral& rhs) const;

    const std::string name;
};

template <typename ValueType>
class ValueLiteral : public Expression {
   public:
    explicit ValueLiteral(const ValueType& value);
    ~ValueLiteral() override = default;

    using value_t = ValueType;

    static TypeHelper::Type type() { return TypeHelper::Type::Unknown; };

    bool operator==(const ValueLiteral& rhs) const;

    const ValueType value;
};
template <typename ValueType>
bool ValueLiteral<ValueType>::operator==(const ValueLiteral& rhs) const {
    return type() == rhs.type() && value == rhs.value;
}
template <typename ValueType>
ValueLiteral<ValueType>::ValueLiteral(const ValueType& value) : value(value) {}

class BoolLiteral : public ValueLiteral<bool>,
                    public std::enable_shared_from_this<BoolLiteral> {
   public:
    explicit BoolLiteral(const std::string& value);

    explicit BoolLiteral(bool value) : ValueLiteral(value) {};

    static TypeHelper::Type type() { return TypeHelper::Type::Bool; };

    void accept(const visitor_t& visitor) override;
};

class IntLiteral : public ValueLiteral<int>,
                   public std::enable_shared_from_this<IntLiteral> {
   public:
    explicit IntLiteral(const std::string& value);

    explicit IntLiteral(int value);

    static TypeHelper::Type type();

    void accept(const visitor_t& visitor) override;
};

class FloatLiteral : public ValueLiteral<float>,
                     public std::enable_shared_from_this<FloatLiteral> {
   public:
    explicit FloatLiteral(const std::string& value);

    explicit FloatLiteral(float value);

    static TypeHelper::Type type();

    void accept(const visitor_t& visitor) override;
};

class StringLiteral : public ValueLiteral<std::string>,
                      public std::enable_shared_from_this<StringLiteral> {
   public:
    explicit StringLiteral(const std::string& value);

    static TypeHelper::Type type();

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

    static OpType inferType(const std::string& value);

    UnaryOperation(const std::string& opType, const expression_t& expression);

    UnaryOperation(OpType opType, expression_t expression);

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

    static OpType inferType(const std::string& value);

    BinaryOperation(const std::string& opType, const expression_t& lhs,
                    const expression_t& rhs);

    BinaryOperation(OpType opType, expression_t lhs, expression_t rhs);

    void accept(const visitor_t& visitor) override;

    OpType type;
    expression_t lhs;
    expression_t rhs;
};

class FunctionCall : public Expression,
                     public std::enable_shared_from_this<FunctionCall> {
   public:
    FunctionCall(const expression_t& callee, const expressions_t& args);

    void accept(const visitor_t& visitor) override;

    expression_t callee;
    expressions_t args;
};

class FormalParameter : public Expression,
                        public std::enable_shared_from_this<FormalParameter> {
   public:
    explicit FormalParameter(std::string identifier);

    bool operator==(const FormalParameter& other) const {
        return identifier == other.identifier;
    }

    void accept(const visitor_t& visitor) override;

    std::string identifier;
};

using formal_parameters = std::vector<std::shared_ptr<FormalParameter>>;

class FunctionDeclaration
    : public Statement,
      public std::enable_shared_from_this<FunctionDeclaration> {
   public:
    FunctionDeclaration(std::string identifier, formal_parameters parameters,
                        statements_t statements);

    void accept(const visitor_t& visitor) override;

    std::string identifier;
    formal_parameters parameters;
    statements_t statements;
};

class IfStatement : public Statement,
                    public std::enable_shared_from_this<IfStatement> {
   public:
    IfStatement(expression_t condition, block_t if_branch, block_t else_branch);

    void accept(const visitor_t& visitor) override;

    expression_t condition;
    block_t if_branch;
    block_t else_branch;
};

class WhileStatement : public Statement,
                       public std::enable_shared_from_this<WhileStatement> {
   public:
    WhileStatement(expression_t condition, block_t block);

    void accept(const visitor_t& visitor) override;

    expression_t condition;
    block_t block;
};

using functions_t = std::vector<std::shared_ptr<FunctionDeclaration>>;

class ClassDeclaration : public Statement,
                         public std::enable_shared_from_this<ClassDeclaration> {
   public:
    ClassDeclaration(std::string identifier, functions_t methods);

    void accept(const visitor_t& visitor) override;

    ~ClassDeclaration() override = default;

    const std::string identifier;
    functions_t methods;
};

class Get : public Expression, public std::enable_shared_from_this<Get> {
   public:
    Get(expression_t expression, std::string identifier);

    void accept(const visitor_t& visitor) override;

    ~Get() override = default;

    const std::string identifier;
    expression_t expression;
};

class Set : public Expression, public std::enable_shared_from_this<Set> {
   public:
    Set(expression_t expression, std::string identifier, expression_t value);

    void accept(const visitor_t& visitor) override;

    ~Set() override = default;

    const std::string identifier;
    expression_t expression;
    expression_t value;
};

class Self : public Expression, public std::enable_shared_from_this<Self> {
   public:
    Self() = default;
    void accept(const visitor_t& visitor) override;

    ~Self() override = default;
};
}  // namespace angreal::parser::AST

#endif  // ANGREAL_AST_H

#pragma clang diagnostic pop