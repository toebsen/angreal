//
// Created by bichlmaier on 07.02.2020.
//

#ifndef ANGREAL_ANGREAL_DFA_H
#define ANGREAL_ANGREAL_DFA_H

#include "dfa.h"
#include "state.h"

namespace angreal::lex {

class AngrealDFA : public DFA<char, State> {
   public:
    AngrealDFA();

   protected:
    void initDFA();

    void addStates();

    void setupInitialState();

    void setupTransitions();

    void setupDigitTransition();

    void setupCommentTransitions();

    void setupIdentifierTransitions();

    void setupStringTransitions();

    void setupRelationalTransitions();

    void setupWhitesSpaceTransitions();
};
}  // namespace angreal::lex

#endif  // ANGREAL_ANGREAL_DFA_H
