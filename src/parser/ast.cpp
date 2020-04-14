//
// Created by bichlmaier on 07.02.2020.
//

#include "ast.h"

#include <boost/lexical_cast.hpp>

namespace tb_lang::parser::AST {

BoolLiteral::BoolLiteral(const std::string& value)
    : ValueLiteral<bool>(value == "true") {}

void BoolLiteral::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
};

IntLiteral::IntLiteral(const std::string& value)
    : ValueLiteral<int>(boost::lexical_cast<int>(value)) {}

void IntLiteral::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
};

FloatLiteral::FloatLiteral(const std::string& value)
    : ValueLiteral<float>(boost::lexical_cast<float>(value)) {}

void FloatLiteral::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
};

StringLiteral::StringLiteral(const std::string& value)
    : ValueLiteral<std::string>(value) {}

void StringLiteral::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
};

void Program::accept(visitor_t visitor) { visitor->visit(shared_from_this()); }

void Block::accept(visitor_t visitor) { visitor->visit(shared_from_this()); }

void ExpressionStatement::accept(visitor_t visitor) { visitor->visit(shared_from_this()); }

void Declaration::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void Assignment::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void Return::accept(visitor_t visitor) { visitor->visit(shared_from_this()); }

void Print::accept(visitor_t visitor) { visitor->visit(shared_from_this()); }


void IdentifierLiteral::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void UnaryOperation::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void BinaryOperation::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void FunctionCall::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void FunctionDeclaration::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}

void FormalParameter::accept(visitor_t visitor) {
    visitor->visit(shared_from_this());
}
}  // namespace tb_lang::parser::AST
