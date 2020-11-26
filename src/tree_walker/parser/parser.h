//
// Created by bichlmaier on 07.02.2020.
//

#ifndef ANGREAL_PARSER_H
#define ANGREAL_PARSER_H

#include <functional>

#include "ast.h"
#include "error_handler.h"
#include "token.h"
#include "type_helper.h"

namespace angreal::parser {

class Parser {
   public:
    explicit Parser(const error_handler_t& error_handler);

    std::shared_ptr<AST::Program> parseProgram(
        const std::vector<Token>& tokens);

    expression_t parseExpression(const std::vector<Token>& tokens);

   protected:
    expression_t parseRelational();

    expression_t parseAssignment(const expression_t& expression);

    expression_t parseAdditive();

    expression_t parseMultiplicative();

    expression_t parseUnary();

    expression_t parsePrimary();

    expression_t parseFunctionCall();

    statement_t parseBlock();

    statement_t parseVariableDeclaration();

    statement_t parseFunctionDeclaration();

    statement_t parseClassDeclaration();

    statement_t parseReturnDeclaration();

    statement_t parsePrintStatement();

    statement_t parseIfStatement();

    statement_t parseWhileStatement();

   private:
    template <typename Type, typename... Args>
    inline std::shared_ptr<Type> MakeASTNode(Args&&... args);

    AST::formal_parameters parseFormalParameters();

    std::shared_ptr<AST::FormalParameter> parseFormalParameter();

    AST::expressions_t parseActualParams();

    std::optional<statement_t> parseStatement();

    void consume();

    void expectToken(Token::Type t) const;

    error_handler_t error_handler_;

    std::vector<Token>::const_iterator current_token;
    std::vector<Token>::const_iterator next_token;
    int current_line_number {0};
};

}  // namespace angreal::parser

#endif  // ANGREAL_PARSER_H
