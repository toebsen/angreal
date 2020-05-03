//
// Created by bichlmaier on 05.02.2020.
//

#ifndef TBLANG_STATE_H
#define TBLANG_STATE_H

#include <map>

namespace tb_lang {
enum class State {
    Invalid = -1,
    Initial = 0,
    Digit,
    DigitPeriod,
    Period,
    AddSub,
    Asterisk,
    Slash,
    StartComment,
    EndComment,
    Identifier,
    StartString,
    EndString,
    Punctuation,
    Equals,
    Exclamation,
    SingleRelational,
    CombinedRelational,
    NewLine,
    WhiteSpace,
    EndOfProgram,
};

const std::map<State, bool> FinalStates = {
    {State::Digit, true},
    {State::DigitPeriod, true},
    {State::Period, false},
    {State::AddSub, true},
    {State::Asterisk, true},
    {State::Slash, true},
    {State::StartComment, false},
    {State::EndComment, true},
    {State::Identifier, true},
    {State::Punctuation, true},
    {State::Equals, true},
    {State::SingleRelational, true},
    {State::CombinedRelational, true},
    {State::StartString, false},
    {State::EndString, true},
    {State::Exclamation, true},
    {State::NewLine, true},
    {State::WhiteSpace, true},
    {State::EndOfProgram, true},
};
}  // namespace tb_lang

#endif  // TBLANG_STATE_H
