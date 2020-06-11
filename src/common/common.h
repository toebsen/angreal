//
// Created by toebs on 04.04.2020.
//

#ifndef ANGREAL_SRC_COMMON_COMMON_H_
#define ANGREAL_SRC_COMMON_COMMON_H_

#include <memory>
#include <string>
#include <vector>

#include "exceptions.h"

namespace angreal {

#ifdef DEBUG_
#define TESTABLE public
#else
#define TESTABLE private
#endif

class Visitor;

using visitor_t = std::shared_ptr<Visitor>;

using string_t = std::string;

namespace parser::AST {
class Node;
class Expression;
class Statement;
class Block;

using node_t = std::shared_ptr<Node>;
using nodes_t = std::vector<node_t>;

using expression_t = std::shared_ptr<Expression>;
using expressions_t = std::vector<expression_t>;

using statement_t = std::shared_ptr<Statement>;
using statements_t = std::vector<statement_t>;
using block_t = std::shared_ptr<Block>;

}  // namespace parser::AST

namespace interpreter::environment {
class Object;
class Type;
class ICallable;
class Instance;
class Environment;

using obj_t = std::shared_ptr<Object>;
using type_t = std::shared_ptr<Type>;
using environment_t = std::shared_ptr<Environment>;

using callable_t = std::shared_ptr<ICallable>;
using instance_t = std::shared_ptr<Instance>;
}  // namespace interpreter::environment

class NonCopyable {
   public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;

   protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
};

// https://dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

// clang-format off
template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;
// clang-format on

}  // namespace angreal

#endif  // ANGREAL_SRC_COMMON_COMMON_H_
