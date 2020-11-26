//
// Created by bichlmaier on 10.02.2020.
//

#ifndef ANGREAL_TYPE_SYSTEM_H
#define ANGREAL_TYPE_SYSTEM_H

#include <memory>
#include <vector>

#include <ast_interfaces.h>
#include <token.h>
#include <visitor.h>

namespace angreal::parser {

class TypeHelper {
   public:
    enum class Type { Bool, Int, Float, String, Unknown };

    static TypeHelper::Type mapTokenToLiteralType(Token::Type expected);

    static std::shared_ptr<AST::Expression> mapTypeToLiteral(
        TypeHelper::Type expected, const std::string& value);
};
}  // namespace angreal::parser

#endif  // ANGREAL_TYPE_SYSTEM_H
