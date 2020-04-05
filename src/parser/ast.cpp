//
// Created by bichlmaier on 07.02.2020.
//

#include "ast.h"

#include <boost/lexical_cast.hpp>

namespace tb_lang::parser::AST {
BoolLiteral::BoolLiteral(const std::string& value)
    : ValueLiteral<bool>(value == "true"){};

IntLiteral::IntLiteral(const std::string& value)
    : ValueLiteral<int>(boost::lexical_cast<int>(value)){};

FloatLiteral::FloatLiteral(const std::string& value)
    : ValueLiteral<float>(boost::lexical_cast<float>(value)){};

StringLiteral::StringLiteral(const std::string& value)
    : ValueLiteral<std::string>(value){};

void Program::accept(visitor_t visitor) { visitor->visit(this); }

void Block::accept(visitor_t visitor) { visitor->visit(this); }

void Declaration::accept(visitor_t visitor) { visitor->visit(this); }

void Assignment::accept(visitor_t visitor) { visitor->visit(this); }

void IdentifierLiteral::accept(visitor_t visitor) { visitor->visit(this); }

void UnaryOperation::accept(visitor_t visitor) { visitor->visit(this); }

void BinaryOperation::accept(visitor_t visitor) { visitor->visit(this); }

void FunctionCall::accept(visitor_t visitor) { visitor->visit(this); }

void FunctionDeclaration::accept(visitor_t visitor) { visitor->visit(this); }

void FormalParameter::accept(visitor_t visitor) { visitor->visit(this); }
}  // namespace tb_lang::parser::AST
