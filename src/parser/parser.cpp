//
// Created by bichlmaier on 07.02.2020.
//

#include "parser.h"

#include <set>

#include <boost/lexical_cast.hpp>
#include <magic_enum.hpp>

#include "type_system.h"

namespace tb_lang::parser {
std::shared_ptr<AST::Program> Parser::parseProgram(
    const std::vector<Token>& tokens) {
    AST::statements_t statements;
    current_token = tokens.begin();
    next_token = tokens.begin() + 1;

    while (current_token->type() != Token::Type::EndOfProgram &&
           current_token != tokens.end()) {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(stmt);
        }
    }
    return std::make_shared<AST::Program>(statements);
}

std::shared_ptr<AST::Expression> Parser::parseExpression(
    const std::vector<Token>& tokens) {
    current_token = tokens.begin();
    next_token = tokens.begin() + 1;
    return parseExpression();
}

void Parser::consume() {
    std::cout << "consuming: " << *next_token << std::endl;
    current_token = next_token;
    next_token = current_token + 1;

    if (current_token->type() == Token::Type::NewLine) {
        ++current_line_number;
        consume();
    }
}

void Parser::expectToken(Token::Type t) {
    if (current_token->type() != t) {
        std::stringstream ss;
        ss << "Expected " << Token::type2str(t)
           << ", but got: " << Token::type2str(current_token->type());
        ss << "('" << current_token->value() << "')"
           << " in line: " << std::to_string(current_line_number);
        error(ss.str());
    }
}

void Parser::expectTokensOneOf(std::initializer_list<Token::Type> types) {
    std::string description;
    for (auto t : types) {
        description += Token::type2str(t) + ", ";
    }

    std::set<Token::Type> allowed(types.begin(), types.end());
    expectToken(description,
                [&](Token::Type t) { return allowed.contains(t); });
}

void Parser::expectToken(const std::string& description,
                         std::function<bool(Token::Type t)> predicate) {
    if (!predicate(current_token->type())) {
        std::stringstream ss;
        ss << "Expected " << description
           << ", but got: " << Token::type2str(current_token->type());
        ss << "('" << current_token->value() << "')"
           << " in line: " << std::to_string(current_line_number);
        error(ss.str());
    }
}

void Parser::error(const std::string& message) {
    std::cout << "ERROR: " << message << std::endl;
    throw RuntimeError(message);
}

std::shared_ptr<AST::Expression> Parser::parseExpression() {
    std::shared_ptr<AST::Expression> expr = parseSimpleExpression();
    if (current_token->type() == Token::Type::RelationalOp) {
        auto opType = current_token->value();
        consume();
        auto rhs = parseSimpleExpression();
        return std::make_shared<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

std::shared_ptr<AST::Expression> Parser::parseSimpleExpression() {
    std::shared_ptr<AST::Expression> expr = parseTerm();
    if (current_token->type() == Token::Type::AdditiveOp ||
        current_token->type() == Token::Type::AndStatement) {
        auto opType = current_token->value();
        consume();
        auto rhs = parseTerm();
        return std::make_shared<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

std::shared_ptr<AST::Expression> Parser::parseTerm() {
    std::shared_ptr<AST::Expression> expr = parseFactor();
    if (current_token->type() == Token::Type::MulOp ||
        current_token->type() == Token::Type::DivOp ||
        current_token->type() == Token::Type::OrStatement) {
        auto opType = current_token->value();
        consume();
        auto rhs = parseFactor();
        return std::make_shared<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

AST::expressions_t Parser::parseActualParams() {
    AST::expressions_t params;
    params.push_back(parseExpression());

    while (current_token->type() == Token::Type::Comma) {
        consume();
        params.push_back(parseExpression());
    }
    return params;
}

std::shared_ptr<AST::FunctionCall> Parser::parserFunctionCall() {
    std::string identifier = current_token->value();
    AST::expressions_t args;
    consume();  // identifier
    expectToken(Token::Type::LeftBracket);
    consume();  //(
    if (current_token->type() != Token::Type::RightBracket) {
        args = parseActualParams();
    }
    expectToken(Token::Type::RightBracket);
    consume();
    return std::make_shared<AST::FunctionCall>(identifier, args);
}

std::shared_ptr<AST::Expression> Parser::parseFactor() {
    if (current_token->type() == Token::Type::Boolean ||
        current_token->type() == Token::Type::Integer ||
        current_token->type() == Token::Type::Float ||
        current_token->type() == Token::Type::String) {
        TypeSystem::Type decl =
            TypeSystem::mapTokenToLiteralType(current_token->type());
        auto value = current_token->value();
        consume();
        return TypeSystem::mapTypeToLiteral(decl, value);
    } else if (current_token->type() == Token::Type::Identifier) {
        if (next_token->type() == Token::Type::LeftBracket) {
            return parserFunctionCall();
        }
        auto value = current_token->value();
        consume();
        return std::make_shared<AST::IdentifierLiteral>(value);
    } else if (current_token->type() == Token::Type::AdditiveOp ||
               current_token->type() == Token::Type::Exclamation) {
        auto opType = current_token->value();
        consume();
        AST::expression_t expression = parseExpression();
        return std::make_shared<AST::UnaryOperation>(opType, expression);
    } else if (current_token->type() == Token::Type::LeftBracket) {
        consume();
        auto expression = parseExpression();
        expectToken(Token::Type::RightBracket);
        consume();
        return expression;
    }
    return nullptr;
}

std::shared_ptr<AST::Declaration> Parser::parseVariableDeclaration() {
    TypeSystem::Type type;
    std::string identifier;
    AST::expression_t expression;
    consume();
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    expectToken(Token::Type::Colon);
    consume();
    type = parseType();
    expectToken(Token::Type::Equal);
    consume();
    expression = parseExpression();
    expectToken(Token::Type::SemiColon);
    consume();
    return std::make_shared<AST::Declaration>(type, identifier, expression);
}

TypeSystem::Type Parser::parseType() {
    expectTokensOneOf({Token::Type::IntIdentifier, Token::Type::BoolIdentifier,
                       Token::Type::FloatIdentifier,
                       Token::Type::StringIdentifier});
    consume();
    return TypeSystem::mapTokenToDeclarationType(current_token->type());
}

std::shared_ptr<AST::Assignment> Parser::parseAssignmentStatement() {
    std::string identifier;
    AST::expression_t expression;
    consume();
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    expectToken(Token::Type::Equal);
    consume();
    expression = parseExpression();
    expectToken(Token::Type::SemiColon);
    consume();
    return std::make_shared<AST::Assignment>(identifier, expression);
}

AST::formal_parameters Parser::parseFormalParameters() {
    AST::formal_parameters parameters;

    expectToken(Token::Type::LeftBracket);
    consume();

    if (current_token->type() != Token::Type::RightBracket) {
        while (current_token->type() != Token::Type::RightBracket) {
            auto param = parseFormalParameter();
            parameters.push_back(param);
            if (current_token->type() != Token::Type::RightBracket) {
                expectToken(Token::Type::Comma);
                consume();
            }
        }
    }

    expectToken(Token::Type::RightBracket);
    consume();
    return parameters;
}

std::shared_ptr<AST::FormalParameter> Parser::parseFormalParameter() {
    expectToken(Token::Type::Identifier);
    std::string identifier = current_token->value();
    consume();
    expectToken(Token::Type::Colon);
    consume();
    expectTokensOneOf({Token::Type::IntIdentifier, Token::Type::BoolIdentifier,
                       Token::Type::FloatIdentifier,
                       Token::Type::StringIdentifier});

    TypeSystem::Type param_type =
        TypeSystem::mapTokenToDeclarationType(current_token->type());
    consume();

    return std::make_shared<AST::FormalParameter>(param_type, identifier);
}

std::shared_ptr<AST::FunctionDeclaration> Parser::parseFunctionDeclaration() {
    std::string identifier;
    TypeSystem::Type return_type;

    consume();  // def
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    auto parameters = parseFormalParameters();

    expectToken(Token::Type::Colon);
    consume();
    expectTokensOneOf({Token::Type::IntIdentifier, Token::Type::BoolIdentifier,
                       Token::Type::FloatIdentifier,
                       Token::Type::StringIdentifier});
    return_type = TypeSystem::mapTokenToDeclarationType(current_token->type());
    consume();

    auto block = parseBlock();

    return std::make_shared<AST::FunctionDeclaration>(
        return_type, identifier, parameters, block->statements);
}

std::shared_ptr<AST::Block> Parser::parseBlock() {
    AST::statements_t statements;
    expectToken(Token::Type::LeftCurlyBracket);
    consume();
    if(current_token->type() == Token::Type::RightCurlyBracket)
    {
        // don't parse empty block
        consume();
        return std::make_shared<AST::Block>(statements);
    }

    do {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(stmt);
        }
    } while (current_token->type() != Token::Type::RightCurlyBracket && current_token->type() != Token::Type::EndOfProgram);
    expectToken(Token::Type::RightCurlyBracket);
    consume();
    return std::make_shared<AST::Block>(statements);
}

std::shared_ptr<AST::Statement> Parser::parseReturnDeclaration() {
    std::string identifier;
    AST::expression_t expression;
    consume();
    expression = parseExpression();
    return std::make_shared<AST::Return>(expression);
}

std::shared_ptr<AST::Statement> Parser::parsePrintStatement() {
    AST::expressions_t args;
    consume();  // identifier
    consume();  //(
    if (current_token->type() != Token::Type::RightBracket) {
        args = parseActualParams();
    }
    expectToken(Token::Type::RightBracket);
    consume();
    return std::make_shared<AST::Print>(args);
}
std::shared_ptr<AST::Statement> Parser::parseStatement() {
    current_line_number = current_token->position().line;

    if (current_token->type() == Token::Type::VarStatement) {
        return parseVariableDeclaration();
    } else if (current_token->type() == Token::Type::SetStatement) {
        return parseAssignmentStatement();
    } else if (current_token->type() == Token::Type::DefStatement) {
        return parseFunctionDeclaration();
    } else if (current_token->type() == Token::Type::PrintStatement) {
        return parsePrintStatement();
    } else if (current_token->type() == Token::Type::LeftCurlyBracket) {
        return parseBlock();
    } else {
        auto expr = parseExpression();
        if (expr) {
            return std::make_shared<ExpressionStatement>(expr);
        } else {
            consume();
        }
    }
    return nullptr;
}

}  // namespace tb_lang::parser
