//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_PARSER_H
#define TBLANG_PARSER_H

#include <functional>

#include "ast.h"
#include "token.h"
#include "type_helper.h"

namespace tb_lang::parser {

class Parser {
   public:
    std::shared_ptr<AST::Program> parseProgram(
        const std::vector<Token>& tokens);

    std::shared_ptr<AST::Expression> parseExpression(
        const std::vector<Token>& tokens);

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

    std::shared_ptr<AST::Statement> parsePrintStatement();

    std::shared_ptr<AST::Statement> parseStatement();

    std::shared_ptr<AST::IfStatement> parseIfStatement();

    std::shared_ptr<AST::WhileStatement> parseWhileStatement();

   private:
    void consume();

    void expectToken(Token::Type t);

    void error(const std::string& message);

    std::vector<Token>::const_iterator current_token;
    std::vector<Token>::const_iterator next_token;
    int current_line_number{0};

    AST::expressions_t parseActualParams();
};

}  // namespace tb_lang::parser

#endif  // TBLANG_PARSER_H
