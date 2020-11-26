//
// Created by bichlmaier on 07.02.2020.
//

#include "angreal_dfa.h"

#include "../common/symbols.h"

namespace angreal::lex {

AngrealDFA::AngrealDFA() : DFA<char, State>(State::Initial, false) {
    initDFA();
}

void AngrealDFA::initDFA() {
    addStates();
    setupInitialState();
    setupTransitions();
}

void AngrealDFA::addStates() {
    for (const auto& [k, v] : FinalStates) {
        addState(k, v);
    }
}

void AngrealDFA::setupInitialState() {
    add_transition(State::Initial, Symbols::Digits(), State::Digit);
    add_transition(State::Initial, Symbols::Period(), State::Period);
    add_transition(State::Initial, Symbols::AddSub(), State::AddSub);
    add_transition(State::Initial, Symbols::Slash(), State::Slash);
    add_transition(State::Initial, Symbols::Asterisk(), State::Asterisk);
    add_transition(State::Initial, Symbols::HashTag(), State::StartComment);
    add_transition(State::Initial, Symbols::UnderScore(), State::Identifier);
    add_transition(State::Initial, Symbols::Letters(), State::Identifier);
    add_transition(State::Initial, Symbols::QuotationMark(),
                   State::StartString);
    add_transition(State::Initial, Symbols::Punctuation(), State::Punctuation);
    add_transition(State::Initial, Symbols::Equals(), State::Equals);
    add_transition(State::Initial, Symbols::Relational(),
                   State::SingleRelational);
    add_transition(State::Initial, Symbols::Exclamation(), State::Exclamation);
    add_transition(State::Initial, Symbols::NewLine(), State::NewLine);
    add_transition(State::Initial, Symbols::Whitespace(), State::WhiteSpace);
}

void AngrealDFA::setupTransitions() {
    setupDigitTransition();
    setupCommentTransitions();
    setupIdentifierTransitions();
    setupStringTransitions();
    setupRelationalTransitions();
    setupWhitesSpaceTransitions();
}

void AngrealDFA::setupDigitTransition() {
    add_transition(State::Digit, Symbols::Digits(), State::Digit);
    add_transition(State::Digit, Symbols::Period(), State::DigitPeriod);
    add_transition(State::DigitPeriod, Symbols::Digits(), State::DigitPeriod);
    add_transition(State::Period, Symbols::Digits(), State::DigitPeriod);
}

void AngrealDFA::setupCommentTransitions() {
    add_transition(State::StartComment, Symbols::Letters(),
                   State::StartComment);
    add_transition(State::StartComment, Symbols::Digits(), State::StartComment);
    add_transition(State::StartComment, Symbols::Whitespace(),
                   State::StartComment);
    add_transition(State::StartComment, Symbols::NewLine(), State::EndComment);
}

void AngrealDFA::setupIdentifierTransitions() {
    add_transition(State::Identifier, Symbols::UnderScore(), State::Identifier);
    add_transition(State::Identifier, Symbols::Letters(), State::Identifier);
    add_transition(State::Identifier, Symbols::Digits(), State::Identifier);
}

void AngrealDFA::setupStringTransitions() {
    add_transition(State::StartString, Symbols::Printable(),
                   State::StartString);
    add_transition(State::StartString, Symbols::QuotationMark(),
                   State::EndString);
}

void AngrealDFA::setupRelationalTransitions() {
    add_transition(State::Exclamation, Symbols::Equals(),
                   State::CombinedRelational);
    add_transition(State::Equals, Symbols::Equals(), State::CombinedRelational);
    add_transition(State::SingleRelational, Symbols::Equals(),
                   State::CombinedRelational);
}

void AngrealDFA::setupWhitesSpaceTransitions() {
    add_transition(State::NewLine, Symbols::NewLine(), State::NewLine);
    add_transition(State::WhiteSpace, Symbols::Whitespace(), State::WhiteSpace);
}

}  // namespace angreal::lex