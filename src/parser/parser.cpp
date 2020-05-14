//
// Created by bichlmaier on 07.02.2020.
//

#include "parser.h"

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
    //    std::cout << "consuming: " << *next_token << std::endl;
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
        TypeHelper::Type decl =
            TypeHelper::mapTokenToLiteralType(current_token->type());
        auto value = current_token->value();
        consume();
        return TypeHelper::mapTypeToLiteral(decl, value);
    }

    if (current_token->type() == Token::Type::Identifier) {
        if (next_token->type() == Token::Type::LeftBracket) {
            return parserFunctionCall();
        }
        auto value = current_token->value();
        consume();
        return std::make_shared<AST::IdentifierLiteral>(value);
    }
    if (current_token->type() == Token::Type::AdditiveOp ||
        current_token->type() == Token::Type::Exclamation) {
        auto opType = current_token->value();
        consume();
        AST::expression_t expression = parseExpression();
        return std::make_shared<AST::UnaryOperation>(opType, expression);
    }
    if (current_token->type() == Token::Type::LeftBracket) {
        consume();
        auto expression = parseExpression();
        expectToken(Token::Type::RightBracket);
        consume();
        return expression;
    }
    return nullptr;
}

std::shared_ptr<AST::Declaration> Parser::parseVariableDeclaration() {
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
    return std::make_shared<AST::Declaration>(identifier, expression);
}

std::shared_ptr<AST::Assignment> Parser::parseAssignmentStatement() {
    std::string identifier;
    AST::expression_t expression;
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

    return std::make_shared<AST::FormalParameter>(identifier);
}

std::shared_ptr<AST::FunctionDeclaration> Parser::parseFunctionDeclaration() {
    std::string identifier;
    consume();
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    auto parameters = parseFormalParameters();

    auto block = parseBlock();

    return std::make_shared<AST::FunctionDeclaration>(identifier, parameters,
                                                      block->statements);
}

std::shared_ptr<AST::Block> Parser::parseBlock() {
    AST::statements_t statements;
    expectToken(Token::Type::LeftCurlyBracket);
    consume();
    if (current_token->type() == Token::Type::RightCurlyBracket) {
        // don't parse empty block
        consume();
        return std::make_shared<AST::Block>(statements);
    }

    do {
        auto stmt = parseStatement();
        if (stmt) {
            statements.push_back(stmt);
        }
    } while (!(current_token->type() == Token::Type::RightCurlyBracket ||
               current_token->type() == Token::Type::EndOfProgram));

    expectToken(Token::Type::RightCurlyBracket);
    consume();
    return std::make_shared<AST::Block>(statements);
}

std::shared_ptr<AST::Statement> Parser::parseReturnDeclaration() {
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
    }
    if (current_token->type() == Token::Type::Identifier &&
        next_token->type() == Token::Type::Equal) {
        return parseAssignmentStatement();
    }
    if (current_token->type() == Token::Type::DefStatement) {
        return parseFunctionDeclaration();
    }
    if (current_token->type() == Token::Type::PrintStatement) {
        return parsePrintStatement();
    }
    if (current_token->type() == Token::Type::LeftCurlyBracket) {
        return parseBlock();
    }
    if (current_token->type() == Token::Type::ReturnStatement) {
        return parseReturnDeclaration();
    }
    if (current_token->type() == Token::Type::IfStatement) {
        return parseIfStatement();
    }
    if (current_token->type() == Token::Type::WhileStatement) {
        return parseWhileStatement();
    }
    if (current_token->type() == Token::Type::EndOfProgram) {
        return nullptr;
    }
    if (current_token->type() == Token::Type::Comment) {
        consume();
        return nullptr;
    }
    auto expr = parseExpression();
    if (expr) {
        return std::make_shared<ExpressionStatement>(expr);
    }
    consume();
    return nullptr;
}
std::shared_ptr<AST::IfStatement> Parser::parseIfStatement() {
    consume();
    expression_t condition = parseExpression();
    block_t block = parseBlock();

    block_t else_block;
    if (current_token->type() == Token::Type::Identifier &&
        current_token->value() == "else") {
        consume();
        else_block = parseBlock();
    }

    return std::make_shared<AST::IfStatement>(condition, block, else_block);
}

std::shared_ptr<AST::WhileStatement> Parser::parseWhileStatement() {
    consume();
    expression_t condition = parseExpression();
    block_t block = parseBlock();

    return std::make_shared<AST::WhileStatement>(condition, block);
}

}  // namespace tb_lang::parser
