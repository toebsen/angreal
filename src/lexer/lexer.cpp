//
// Created by bichlmaier on 05.02.2020.
//

#include "lexer.h"

#include <boost/algorithm/string.hpp>

namespace tb_lang::lex {

std::vector<Token> Lexer::lex(const std::string& program) {
    std::string _program = program;

    std::vector<Token> tokens;
    std::string lexeme;
    dfa_.reset();

    _program.push_back('\n');

    Token::Position pos{0};
    for (auto it = _program.begin(); it != _program.end() - 1; ++it) {
        auto c = *it;
        auto c_next = *(it + 1);
        lexeme.push_back(c);
        dfa_.input(c);
        if (dfa_.isAccepted() && !dfa_.acceptable(c_next)) {
            if (dfa_.state() != State::WhiteSpace) {
                boost::replace_all(lexeme, "\n", R"(\n)");
                if (dfa_.state() == State::EndString) {
                    boost::replace_all(lexeme, "\"", "");
                }
                tokens.emplace_back(Token{lexeme, dfa_.state(), pos});
            }

            lexeme.clear();
            dfa_.reset();
        }

        if (c == '\n') {
            ++pos.line;
        }
    }
    tokens.emplace_back("<EOP>", State::EndOfProgram, pos);

    return tokens;
}

}  // namespace tb_lang::lex