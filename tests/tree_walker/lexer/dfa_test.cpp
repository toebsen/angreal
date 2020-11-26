//
// Created by bichlmaier on 05.02.2020.
//

#include "lexer/dfa.h"

#include <gtest/gtest.h>

using namespace angreal::lex;

enum class State {
    Invalid = -1,
    S0,
    S1,
    S2,
};

typedef DFA<char, State> DFA_t;

TEST(DFA_Test, NoTransition) {
    DFA_t dfa(State::S0, true);
    dfa.input('1');
    EXPECT_EQ(State::Invalid, dfa.state());
    EXPECT_FALSE(dfa.isAccepted());
}

TEST(DFA_Test, UnknownStateException) {
    DFA_t dfa(State::S0, true);

    EXPECT_THROW(dfa.add_transition(State::S0, {'0'}, State::S1),
                 DFA_t::UnknownStateException);
    EXPECT_THROW(dfa.add_transition(State::S2, {'0'}, State::S0),
                 DFA_t::UnknownStateException);
}

TEST(DFA_Test, StateAlreadyKnownException) {
    DFA_t dfa(State::S0, true);

    EXPECT_THROW(dfa.addState(State::S0, true),
                 DFA_t::StateAlreadyKnownException);
    EXPECT_NO_THROW(dfa.addState(State::S1, true));
}

TEST(DFA_Test, SingleTransition) {
    DFA_t dfa(State::S0, false);
    dfa.addState(State::S1, true);

    dfa.addTransition(State::S0, '1', State::S1);
    dfa.input('1');
    EXPECT_EQ(State::S1, dfa.state());
    dfa.reset();
    EXPECT_EQ(State::S0, dfa.state());
}

TEST(DFA_Test, SmallAutomaton) {
    DFA_t dfa(State::S0, false);

    dfa.addState(State::S1, true);

    dfa.addTransition(State::S0, '0', State::S0);
    dfa.addTransition(State::S0, '1', State::S1);
    dfa.addTransition(State::S1, '0', State::S0);

    dfa.input('0');
    EXPECT_EQ(State::S0, dfa.state());
    dfa.input('1');
    EXPECT_EQ(State::S1, dfa.state());
    EXPECT_TRUE(dfa.isAccepted());
    dfa.input('0');
    EXPECT_EQ(State::S0, dfa.state());
    EXPECT_FALSE(dfa.isAccepted());
}

TEST(DFA_Test, BinaryAutomaton) {
    //
    //    Start -> S0 -[0,1]-> (S1) -----|
    //                        /|\      |
    //                         |-[0,1]-|

    DFA_t dfa(State::S0, false);

    dfa.addState(State::S1, true);

    dfa.add_transition(State::S0, {'0', '1'}, State::S1);
    dfa.add_transition(State::S1, {'0', '1'}, State::S1);

    // adding an arbitrary binary number
    EXPECT_EQ(State::S0, dfa.state());
    dfa.input('0');
    EXPECT_EQ(State::S1, dfa.state());
    dfa.input('1');
    dfa.input('1');
    dfa.input('0');
    EXPECT_TRUE(dfa.acceptable('0'));
    EXPECT_FALSE(dfa.acceptable('2'));
    dfa.input('1');
    EXPECT_EQ(State::S1, dfa.state());
    EXPECT_TRUE(dfa.isAccepted());

    // lets provoke an error
    dfa.input('2');
    EXPECT_EQ(State::Invalid, dfa.state());
    EXPECT_FALSE(dfa.isAccepted());
}
