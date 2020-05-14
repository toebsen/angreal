//
// Created by bichlmaier on 05.02.2020.
//

#ifndef ANGREAL_LEXER_H
#define ANGREAL_LEXER_H

#include <array>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <token.h>

#include "angreal_dfa.h"

namespace angreal::lex {

class Lexer {
   public:
    std::vector<Token> lex(const std::string& program);

   private:
    AngrealDFA dfa_;
};
}  // namespace angreal::lex

#endif  // ANGREAL_LEXER_H
