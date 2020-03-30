//
// Created by bichlmaier on 05.02.2020.
//

#ifndef TBLANG_LEXER_H
#define TBLANG_LEXER_H

#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <token.h>

#include "tb_dfa.h"

namespace tb_lang::lex {

class Lexer {
   public:
    virtual ~Lexer() = default;

    std::vector<Token> lex(const std::string &program);

   protected:
    TB_DFA dfa_;
};
}  // namespace tb_lang::lex

#endif  // TBLANG_LEXER_H
