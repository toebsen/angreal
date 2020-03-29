//
// Created by bichlmaier on 05.02.2020.
//

#ifndef TBLANG_DFA_H
#define TBLANG_DFA_H

#include <string>

#include <set>
#include <map>
#include <magic_enum.hpp>

namespace tb_lang::lex {

/**
 * Discrete finite-state Automaton
 *
 *  inspired by: https://github.com/fffaraz/DFA
 *
 * @tparam Input Input type e.g. char
 * @tparam State -> State Enum type [note: needs valid State(-1) for invalid state]
 */

template<typename Input, typename State>
class DFA
{
  typedef std::multimap<std::pair<State, Input>, State> transition_container_t;
  typedef std::set<State> state_container;

public:
  class UnknownStateException;

  class StateAlreadyKnownException;

  DFA(const State initial_state, bool is_initial_final)
  {
    this->initial_state_ = this->current_state_ = initial_state;
    addState(initial_state, is_initial_final);
    if (initial_state != State(-1)) {
      addState(State(-1), false);// invalid dead state
    }
  }

  virtual ~DFA() = default;

  void addState(const State s, bool is_final)
  {
    if (states_.contains(s)) {
      throw StateAlreadyKnownException(s);
    }

    states_.insert(s);
    if (is_final) {
      final_states_.insert(s);
    }
  }

  void addTransition(const State src, const Input input, const State dest)
  {
    if (!states_.contains(src)) throw UnknownStateException(src);
    if (!states_.contains(dest)) throw UnknownStateException(dest);

    transitions_.insert({ { src, input }, dest });
  }

  void add_transition(const State src, const std::initializer_list<Input> inputs, const State dest)
  {
    for (auto i : inputs)
      addTransition(src, i, dest);
  }

  void add_transition(const State src, const std::set<Input> inputs, const State dest)
  {
    for (auto i : inputs)
      addTransition(src, i, dest);
  }

  void reset()
  {
    current_state_ = initial_state_;
  }

  State input(const Input input)
  {
    if (auto it = transitions_.find({ current_state_, input }); it != transitions_.end()) {
      return current_state_ = it->second;
    } else {
      return current_state_ = State(-1);
    }
  }

  bool isAccepted()
  {
    return final_states_.contains(current_state_);
  }

  bool acceptable(const Input input)
  {
    auto tr = std::make_pair(current_state_, input);
    if (auto it = transitions_.find(tr); it != transitions_.end()) {
      return final_states_.contains(it->second);
    }
    return false;
  }

  State state()
  {
    return current_state_;
  }

protected:
  State initial_state_;
  State current_state_;
  state_container states_;
  state_container final_states_;
  transition_container_t transitions_;
};

template<typename Input, typename State>
class DFA<Input, State>::UnknownStateException : public std::exception
{
public:
  explicit UnknownStateException(const State &state)
    : msg_("Unknown state: " + std::string(magic_enum::enum_name(state))) {}

  [[nodiscard]] char const *what() const noexcept override { return msg_.c_str(); }

protected:
  std::string msg_;
};

template<typename Input, typename State>
class DFA<Input, State>::StateAlreadyKnownException : public std::exception
{
public:
  explicit StateAlreadyKnownException(const State &state)
    : msg_("State already known: " + std::string(magic_enum::enum_name(state))) {}

  [[nodiscard]] char const *what() const noexcept override { return msg_.c_str(); }

protected:
  std::string msg_;
};
}// namespace tb_lang::lex

#endif//TBLANG_DFA_H
