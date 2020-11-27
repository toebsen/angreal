//
// Created by toebsen on 27.11.2020.
//
#ifndef ANGREAL_SRC_LIBANGREAL_STACK_H_
#define ANGREAL_SRC_LIBANGREAL_STACK_H_

#include <deque>
#include <stack>

namespace angreal {
template <typename T, typename Container = std::deque<T>>
class Stack : public std::stack<T, Container> {
    using std::stack<T, Container>::c;

   public:
    inline constexpr auto cbegin() const { return std::cbegin(c); }
    inline constexpr auto cend() const { return std::cend(c); }

    inline auto pop_one() {
        auto val = this->top();
        this->pop();
        return val;
    }
    void reset() { c.clear(); };
};
}  // namespace angreal
#endif  // ANGREAL_SRC_LIBANGREAL_STACK_H_
