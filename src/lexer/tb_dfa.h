//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_TB_DFA_H
#define TBLANG_TB_DFA_H

#include "dfa.h"
#include "state.h"

namespace tb_lang::lex
{

    class TB_DFA : public DFA<char, State>
    {
    public:
        TB_DFA();

        ~TB_DFA() = default;

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
}

#endif //TBLANG_TB_DFA_H
