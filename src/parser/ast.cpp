//
// Created by bichlmaier on 07.02.2020.
//

#include "ast.h"
#include <boost/lexical_cast.hpp>

namespace tb_lang::parser::AST {
BoolLiteral::BoolLiteral(const std::string &value)
  : ValueLiteral<bool>(value == "true"){};

IntLiteral::IntLiteral(const std::string &value)
  : ValueLiteral<int>(boost::lexical_cast<int>(value)){};

FloatLiteral::FloatLiteral(const std::string &value)
  : ValueLiteral<float>(boost::lexical_cast<float>(value)){};

StringLiteral::StringLiteral(const std::string &value)
  : ValueLiteral<std::string>(value){};

}// namespace tb_lang::parser::AST
