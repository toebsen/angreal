//
// Created by bichlmaier on 07.02.2020.
//

#include "parser.h"

namespace angreal::parser {

Parser::Parser(const error_handler_t& error_handler)
    : error_handler_(error_handler) {}

template <typename Type, typename... Args>
std::shared_ptr<Type> Parser::MakeASTNode(Args&&... args) {
    {
        auto node = std::make_shared<Type>(std::forward<Args>(args)...);
        node->SetLine(current_line_number);
        return node;
    }
}

std::shared_ptr<AST::Program> Parser::parseProgram(
    const std::vector<Token>& tokens) {
    AST::statements_t statements;
    current_token = tokens.begin();
    next_token = tokens.begin() + 1;

    while (current_token->type() != Token::Type::EndOfProgram &&
           current_token != tokens.end()) {
        if (auto stmt = parseStatement()) {
            statements.push_back(stmt.value());
        }
    }
    return MakeASTNode<AST::Program>(statements);
}

expression_t Parser::parseExpression(const std::vector<Token>& tokens) {
    current_token = tokens.begin();
    next_token = tokens.begin() + 1;
    return parseRelational();
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

void Parser::expectToken(Token::Type t) const {
    if (current_token->type() != t) {
        std::stringstream ss;
        ss << "Expected " << Token::type2str(t)
           << ", but got: " << Token::type2str(current_token->type());
        error_handler_->ParserError(ss.str(), *current_token);
    }
}

expression_t Parser::parseRelational() {
    expression_t expr = parseAdditive();
    if (current_token->type() == Token::Type::RelationalOp) {
        auto opType = current_token->value();
        consume();
        auto rhs = parseAdditive();
        return MakeASTNode<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

expression_t Parser::parseAdditive() {
    expression_t expr = parseMultiplicative();
    if (current_token->type() == Token::Type::AdditiveOp ||
        current_token->type() == Token::Type::AndStatement) {
        auto opType = current_token->value();
        consume();
        auto rhs = parseMultiplicative();
        return MakeASTNode<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

expression_t Parser::parseMultiplicative() {
    expression_t expr = parseUnary();
    if (current_token->type() == Token::Type::MulOp ||
        current_token->type() == Token::Type::DivOp ||
        current_token->type() == Token::Type::OrStatement) {
        auto opType = current_token->value();
        consume();
        auto rhs = parsePrimary();
        return MakeASTNode<AST::BinaryOperation>(opType, expr, rhs);
    }
    return expr;
}

expression_t Parser::parseUnary() {
    if (current_token->type() == Token::Type::AdditiveOp ||
        current_token->type() == Token::Type::Exclamation) {
        auto opType = current_token->value();
        consume();
        AST::expression_t expression = parseRelational();
        return MakeASTNode<AST::UnaryOperation>(opType, expression);
    }

    return parseFunctionCall();
}

AST::expressions_t Parser::parseActualParams() {
    AST::expressions_t params;
    params.push_back(parseRelational());

    while (current_token->type() == Token::Type::Comma) {
        consume();
        params.push_back(parseRelational());
    }
    return params;
}

expression_t Parser::parseFunctionCall() {
    auto expression = parsePrimary();

    while (true) {
        if (current_token->type() == Token::Type::LeftBracket) {
            consume();  //(
            AST::expressions_t args;
            if (current_token->type() != Token::Type::RightBracket) {
                args = parseActualParams();
            }
            expectToken(Token::Type::RightBracket);
            consume();
            expression = MakeASTNode<AST::FunctionCall>(expression, args);
        } else if (current_token->type() == Token::Type::Dot) {
            consume();
            expectToken(Token::Type::Identifier);
            auto identifier = current_token->value();
            consume();
            expression = MakeASTNode<AST::Get>(expression, identifier);
        } else {
            break;
        }
    }

    return expression;
}

expression_t Parser::parsePrimary() {
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

    if (current_token->type() == Token::Type::SelfStatement) {
        consume();
        return MakeASTNode<AST::Self>();
    }

    if (current_token->type() == Token::Type::Identifier) {
        auto value = current_token->value();
        consume();
        if (value == "super") {
            expectToken(Token::Type::Dot);
            consume();
            expectToken(Token::Type::Identifier);
            auto ident = current_token->value();
            consume();
            return MakeASTNode<AST::Super>(ident);
        }
        return MakeASTNode<AST::IdentifierLiteral>(value);
    }

    if (current_token->type() == Token::Type::LeftBracket) {
        consume();
        auto expression = parseRelational();
        expectToken(Token::Type::RightBracket);
        consume();
        return expression;
    }

    return nullptr;
}

statement_t Parser::parseVariableDeclaration() {
    std::string identifier;
    AST::expression_t expression;
    consume();
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    expectToken(Token::Type::Equal);
    consume();
    expression = parseRelational();
    return MakeASTNode<AST::Declaration>(identifier, expression);
}

expression_t Parser::parseAssignment(const expression_t& expression) {
    expectToken(Token::Type::Equal);
    consume();
    auto value = parseRelational();

    if (auto ident = std::dynamic_pointer_cast<IdentifierLiteral>(expression)) {
        return MakeASTNode<AST::Assignment>(ident->name, value);
    }
    if (auto get = std::dynamic_pointer_cast<Get>(expression)) {
        return MakeASTNode<AST::Set>(get->expression, get->identifier, value);
    }
    return nullptr;
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

    return MakeASTNode<AST::FormalParameter>(identifier);
}

statement_t Parser::parseFunctionDeclaration() {
    std::string identifier;
    consume();
    expectToken(Token::Type::Identifier);
    identifier = current_token->value();
    consume();
    auto parameters = parseFormalParameters();

    auto block = std::dynamic_pointer_cast<Block>(parseBlock());

    return MakeASTNode<AST::FunctionDeclaration>(identifier, parameters,
                                                 block->statements);
}

statement_t Parser::parseBlock() {
    AST::statements_t statements;
    expectToken(Token::Type::LeftCurlyBracket);
    consume();
    if (current_token->type() == Token::Type::RightCurlyBracket) {
        // don't parse empty block
        consume();
        return MakeASTNode<AST::Block>(statements);
    }

    do {
        if (auto stmt = parseStatement()) {
            statements.push_back(stmt.value());
        }
    } while (!(current_token->type() == Token::Type::RightCurlyBracket ||
               current_token->type() == Token::Type::EndOfProgram));

    expectToken(Token::Type::RightCurlyBracket);
    consume();
    return MakeASTNode<AST::Block>(statements);
}

statement_t Parser::parseReturnDeclaration() {
    AST::expression_t expression;
    consume();
    expression = parseRelational();
    return MakeASTNode<AST::Return>(expression);
}

statement_t Parser::parsePrintStatement() {
    AST::expressions_t args;
    consume();  // identifier
    consume();  //(
    if (current_token->type() != Token::Type::RightBracket) {
        args = parseActualParams();
    }
    expectToken(Token::Type::RightBracket);
    consume();
    return MakeASTNode<AST::Print>(args);
}

std::optional<statement_t> Parser::parseStatement() {
    current_line_number = current_token->position().line;

    if (current_token->type() == Token::Type::VarStatement) {
        return parseVariableDeclaration();
    }
    if (current_token->type() == Token::Type::DefStatement) {
        return parseFunctionDeclaration();
    }
    if (current_token->type() == Token::Type::ClassStatement) {
        return parseClassDeclaration();
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
        return std::nullopt;
    }
    if (current_token->type() == Token::Type::Comment) {
        consume();
        return std::nullopt;
    }

    auto expr = parseRelational();
    if (current_token->type() == Token::Type::Equal) {
        expr = parseAssignment(expr);
    }

    if (expr) {
        return MakeASTNode<ExpressionStatement>(expr);
    }

    consume();
    return std::nullopt;
}

statement_t Parser::parseIfStatement() {
    consume();
    expression_t condition = parseRelational();
    block_t if_branch = std::dynamic_pointer_cast<Block>(parseBlock());

    block_t else_branch;
    if (current_token->type() == Token::Type::Identifier &&
        current_token->value() == "else") {
        consume();
        else_branch = std::dynamic_pointer_cast<Block>(parseBlock());
    }

    return MakeASTNode<AST::IfStatement>(condition, if_branch, else_branch);
}

statement_t Parser::parseWhileStatement() {
    consume();
    expression_t condition = parseRelational();
    block_t block = std::dynamic_pointer_cast<Block>(parseBlock());

    return MakeASTNode<AST::WhileStatement>(condition, block);
}

statement_t Parser::parseClassDeclaration() {
    consume();  // class

    expectToken(Token::Type::Identifier);
    auto identifier = *current_token;
    consume();

    std::optional<identifier_t> superclass;
    if (current_token->type() == Token::Type::LeftBracket) {
        // optional superclass
        consume();

        expectToken(Token::Type::Identifier);
        superclass = MakeASTNode<IdentifierLiteral>(current_token->value());
        consume();

        expectToken(Token::Type::RightBracket);
        consume();
    }

    expectToken(Token::Type::LeftCurlyBracket);
    consume();

    functions_t methods;
    while (true) {
        if (current_token->type() == Token::Type::RightCurlyBracket) {
            break;
        }

        if (current_token->type() == Token::Type::DefStatement) {
            methods.push_back(std::dynamic_pointer_cast<FunctionDeclaration>(
                parseFunctionDeclaration()));
        } else {
            error_handler_->ParserError(
                "Only function declarations are allowed inside declaration of "
                "class !",
                identifier);
            break;
        }
    }

    expectToken(Token::Type::RightCurlyBracket);
    consume();

    return MakeASTNode<AST::ClassDeclaration>(identifier.value(), methods,
                                              superclass);
}

}  // namespace angreal::parser