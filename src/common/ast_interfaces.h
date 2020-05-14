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

class Node : private NonCopyable {
   public:
    ~Node() override = default;

    virtual void accept(const visitor_t& visitor) = 0;
};

class Statement : public Node {
   public:
    void accept(const visitor_t& visitor) override = 0;

    ~Statement() override = default;
};

class Expression : public Node {
   public:
    void accept(const visitor_t& visitor) override = 0;

    ~Expression() override = default;
};

}  // namespace tb_lang::parser::AST
#endif  // TBLANG_AST_INTERFACES_H

#pragma clang diagnostic pop