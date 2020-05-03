#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-special-member-functions"
//
// Created by bichlmaier on 10.02.2020.
//

#ifndef TBLANG_AST_INTERFACES_H
#define TBLANG_AST_INTERFACES_H

#include <memory>
#include <vector>

#include "common.h"
#include "visitor.h"

namespace tb_lang::parser::AST {

using visitor_t = std::shared_ptr<Visitor>;

   class Node : private NonCopyable {
   public:
    ~Node() override = default;

    virtual void accept(const visitor_t& visitor) = 0;
};

class Statement : public Node {
   public:
    virtual void accept(const visitor_t& visitor) = 0;

    ~Statement() override = default;
};

class Expression : public Node {
   public:
    virtual void accept(const visitor_t& visitor) = 0;

    ~Expression() override = default;
};

using node_t = std::shared_ptr<Node>;
using nodes_t = std::vector<node_t>;

using expression_t = std::shared_ptr<Expression>;
using expressions_t = std::vector<expression_t>;

using statement_t = std::shared_ptr<Statement>;
using statements_t = std::vector<statement_t>;

}  // namespace tb_lang::parser::AST
#endif  // TBLANG_AST_INTERFACES_H

#pragma clang diagnostic pop