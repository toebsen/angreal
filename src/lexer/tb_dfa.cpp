//
// Created by bichlmaier on 07.02.2020.
//

#include "tb_dfa.h"
#include "../common/symbols.h"

namespace tb_lang::lex {

TB_DFA::TB_DFA() : DFA<char, State>(State::Initial, false) {
    initDFA();
}

void TB_DFA::initDFA() {
    addStates();
    setupInitialState();
    setupTransitions();
}

void TB_DFA::addStates() {
    for (const auto &[k, v] : FinalStates) {
        addState(k, v);
    }
}

void TB_DFA::setupInitialState() {
    add_transition(State::Initial, Symbols::Digits(), State::Digit);
    add_transition(State::Initial, Symbols::Period(), State::Period);
    add_transition(State::Initial, Symbols::AddSub(), State::AddSub);
    add_transition(State::Initial, Symbols::Slash(), State::Slash);
    add_transition(State::Initial, Symbols::Asterisk(), State::Asterisk);
    add_transition(State::Initial, Symbols::HashTag(), State::StartComment);
    add_transition(State::Initial, Symbols::UnderScore(), State::Identifier);
    add_transition(State::Initial, Symbols::Letters(), State::Identifier);
    add_transition(State::Initial, Symbols::QuotationMark(), State::StartString);
    add_transition(State::Initial, Symbols::Punctuation(), State::Punctuation);
    add_transition(State::Initial, Symbols::Equals(), State::Equals);
    add_transition(State::Initial, Symbols::Relational(), State::SingleRelational);
    add_transition(State::Initial, Symbols::Exclamation(), State::Exclamation);
    add_transition(State::Initial, Symbols::NewLine(), State::NewLine);
    add_transition(State::Initial, Symbols::Whitespace(), State::WhiteSpace);
}

void TB_DFA::setupTransitions() {
    setupDigitTransition();
    setupCommentTransitions();
    setupIdentifierTransitions();
    setupStringTransitions();
    setupRelationalTransitions();
    setupWhitesSpaceTransitions();
}

void TB_DFA::setupDigitTransition() {
    add_transition(State::Digit, Symbols::Digits(), State::Digit);
    add_transition(State::Digit, Symbols::Period(), State::DigitPeriod);
    add_transition(State::DigitPeriod, Symbols::Digits(), State::DigitPeriod);
    add_transition(State::Period, Symbols::Digits(), State::DigitPeriod);
}

void TB_DFA::setupCommentTransitions() {
    add_transition(State::StartComment, Symbols::Letters(), State::StartComment);
    add_transition(State::StartComment, Symbols::Digits(), State::StartComment);
    add_transition(State::StartComment, Symbols::Whitespace(), State::StartComment);
    add_transition(State::StartComment, Symbols::NewLine(), State::EndComment);

}

void TB_DFA::setupIdentifierTransitions() {
    add_transition(State::Identifier, Symbols::UnderScore(), State::Identifier);
    add_transition(State::Identifier, Symbols::Letters(), State::Identifier);

}

void TB_DFA::setupStringTransitions() {
    add_transition(State::StartString, Symbols::Letters(), State::StartString);
    add_transition(State::StartString, Symbols::Digits(), State::StartString);
    add_transition(State::StartString, Symbols::Whitespace(), State::StartString);
    add_transition(State::StartString, Symbols::QuotationMark(), State::EndString);

}

void TB_DFA::setupRelationalTransitions() {
    add_transition(State::Exclamation, Symbols::Equals(), State::CombinedRelational);
    add_transition(State::Equals, Symbols::Equals(), State::CombinedRelational);
    add_transition(State::SingleRelational, Symbols::Equals(), State::CombinedRelational);

}

void TB_DFA::setupWhitesSpaceTransitions() {
    add_transition(State::NewLine, Symbols::NewLine(), State::NewLine);
    add_transition(State::WhiteSpace, Symbols::Whitespace(), State::WhiteSpace);
}

}