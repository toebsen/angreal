//
// Created by bichlmaier on 10.02.2020.
//

#ifndef TBLANG_AST_INTERFACES_H
#define TBLANG_AST_INTERFACES_H

#include <vector>
#include <memory>

#include <visitor.h>


namespace tb_lang::parser::AST {

class Node
{
public:
  virtual void accept(Visitor *) = 0;

  virtual ~Node() = default;
};

class Statement : public Node
{
public:
  virtual void accept(Visitor *visitor) = 0;

  virtual ~Statement() = default;
};

class Expression : public Node
{
public:
  virtual void accept(Visitor *visitor) = 0;

  virtual ~Expression() = default;
};

typedef std::shared_ptr<Node> node_t;
typedef std::vector<node_t> nodes_t;

typedef std::shared_ptr<Expression> expression_t;
typedef std::vector<expression_t> expressions_t;

typedef std::shared_ptr<Statement> statement_t;
typedef std::vector<statement_t> statements_t;

}// namespace tb_lang::parser::AST
#endif//TBLANG_AST_INTERFACES_H
