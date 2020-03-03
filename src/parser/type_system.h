//
// Created by bichlmaier on 10.02.2020.
//

#ifndef TBLANG_TYPE_SYSTEM_H
#define TBLANG_TYPE_SYSTEM_H

#include <vector>
#include <memory>

#include <ast_interfaces.h>
#include <visitor.h>
#include <token.h>

namespace tb_lang::parser
{

    class TypeSystem
    {
    public:
        enum class Type
        {
            Bool,
            Int,
            Float,
            String,
            Unknown
        };


        static TypeSystem::Type mapTokenToLiteralType(Token::Type expected);

        static Token::Type mapLiteralTypeToToken(TypeSystem::Type expected);

        static TypeSystem::Type mapTokenToDeclarationType(Token::Type token);

        static std::shared_ptr<AST::Expression>
        mapTypeToLiteral(TypeSystem::Type expected, const std::string &value);

    };
}


#endif //TBLANG_TYPE_SYSTEM_H
