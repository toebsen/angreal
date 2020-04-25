//
// Created by bichlmaier on 07.02.2020.
//

#include "ast.h"

#include <boost/lexical_cast.hpp>

namespace tb_lang::parser::AST {

BoolLiteral::BoolLiteral(const std::string& value)
    : ValueLiteral<bool>(value == "true") {}

void BoolLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
};

IntLiteral::IntLiteral(const std::string& value)
    : ValueLiteral<int>(boost::lexical_cast<int>(value)) {}

void IntLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
};

FloatLiteral::FloatLiteral(const std::string& value)
    : ValueLiteral<float>(boost::lexical_cast<float>(value)) {}

void FloatLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
};

StringLiteral::StringLiteral(const std::string& value)
    : ValueLiteral<std::string>(value) {}

void StringLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
};

void Program::accept(const visitor_t& visitor) { visitor->visit(shared_from_this()); }

void Block::accept(const visitor_t& visitor) { visitor->visit(shared_from_this()); }

void ExpressionStatement::accept(const visitor_t& visitor) { visitor->visit(shared_from_this()); }

void Declaration::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void Assignment::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void Return::accept(const visitor_t& visitor) { visitor->visit(shared_from_this()); }

void Print::accept(const visitor_t& visitor) { visitor->visit(shared_from_this()); }


void IdentifierLiteral::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void UnaryOperation::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void BinaryOperation::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void FunctionCall::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void FunctionDeclaration::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}

void FormalParameter::accept(const visitor_t& visitor) {
    visitor->visit(shared_from_this());
}
}  // namespace tb_lang::parser::AST
