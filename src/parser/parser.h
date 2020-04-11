//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_PARSER_H
#define TBLANG_PARSER_H

#include <functional>

#include "ast.h"
#include "token.h"
#include "type_system.h"

namespace tb_lang::parser {

class Parser {
   public:
    virtual ~Parser() = default;

    std::shared_ptr<AST::Program> parseProgram(
        const std::vector<Token> &tokens);

    std::shared_ptr<AST::Expression> parseExpression(
        const std::vector<Token> &tokens);

   protected:
    std::shared_ptr<AST::FunctionDeclaration> parseFunctionDeclaration();

    AST::formal_parameters parseFormalParameters();

    std::shared_ptr<AST::FormalParameter> parseFormalParameter();

    std::shared_ptr<AST::Block> parseBlock();

    std::shared_ptr<AST::Expression> parseExpression();

    std::shared_ptr<AST::Declaration> parseVariableDeclaration();

    std::shared_ptr<AST::Assignment> parseAssignmentStatement();

    std::shared_ptr<AST::Expression> parseSimpleExpression();

    std::shared_ptr<AST::Expression> parseTerm();

    std::shared_ptr<AST::Expression> parseFactor();

    std::shared_ptr<AST::FunctionCall> parserFunctionCall();

    std::shared_ptr<AST::Statement> parseReturnDeclaration();

    std::shared_ptr<AST::Expression> parseTypeLiteral(
        TypeSystem::Type expected);

    TypeSystem::Type parseType();

    void consume();

    void expectToken(Token::Type t);

    void expectTokensOneOf(std::initializer_list<Token::Type> types);

    void expectToken(const std::string &description,
                     std::function<bool(Token::Type t)> predicate);

    void error(const std::string &message);

    std::vector<Token>::const_iterator current_token;
    std::vector<Token>::const_iterator next_token;
    int current_line_number{0};

    AST::expressions_t parseActualParams();

};

}  // namespace tb_lang::parser

#endif  // TBLANG_PARSER_H
