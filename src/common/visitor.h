//
// Created by bichlmaier on 07.02.2020.
//

#ifndef ANGREAL_VISITOR_H
#define ANGREAL_VISITOR_H

#include <memory>
#include <string>

#include "common.h"

namespace angreal {

namespace parser::AST {
class Program;
class Block;
class ExpressionStatement;
class Declaration;
class Assignment;
class Return;
class IdentifierLiteral;
class IdentifierLiteral;
class BoolLiteral;
class IntLiteral;
class FloatLiteral;
class StringLiteral;
class UnaryOperation;
class BinaryOperation;
class FunctionCall;
class FormalParameter;
class FunctionDeclaration;
class IfStatement;
class WhileStatement;
class Print;
class ClassDeclaration;
class Get;
class Set;
class Self;
class Super;
}  // namespace parser::AST

using namespace parser::AST;

class Visitor : public std::enable_shared_from_this<Visitor>,
                private NonCopyable {
   public:
    virtual void visit(const std::shared_ptr<Program>& node) = 0;
    virtual void visit(const std::shared_ptr<Block>& node) = 0;
    virtual void visit(const std::shared_ptr<ExpressionStatement>& node) = 0;
    virtual void visit(const std::shared_ptr<Declaration>& node) = 0;
    virtual void visit(const std::shared_ptr<Assignment>& node) = 0;
    virtual void visit(const std::shared_ptr<Return>& node) = 0;
    virtual void visit(const std::shared_ptr<Print>& node) = 0;
    virtual void visit(const std::shared_ptr<IdentifierLiteral>& node) = 0;
    virtual void visit(const std::shared_ptr<BoolLiteral>& node) = 0;
    virtual void visit(const std::shared_ptr<IntLiteral>& node) = 0;
    virtual void visit(const std::shared_ptr<FloatLiteral>& node) = 0;
    virtual void visit(const std::shared_ptr<StringLiteral>& node) = 0;
    virtual void visit(const std::shared_ptr<UnaryOperation>& node) = 0;
    virtual void visit(const std::shared_ptr<BinaryOperation>& node) = 0;
    virtual void visit(const std::shared_ptr<FunctionCall>& node) = 0;
    virtual void visit(const std::shared_ptr<FormalParameter>& node) = 0;
    virtual void visit(const std::shared_ptr<FunctionDeclaration>& node) = 0;
    virtual void visit(const std::shared_ptr<IfStatement>& node) = 0;
    virtual void visit(const std::shared_ptr<WhileStatement>& node) = 0;
    virtual void visit(const std::shared_ptr<ClassDeclaration>& node) = 0;
    virtual void visit(const std::shared_ptr<Get>& node) = 0;
    virtual void visit(const std::shared_ptr<Set>& node) = 0;
    virtual void visit(const std::shared_ptr<Self>& node) = 0;
    virtual void visit(const std::shared_ptr<Super>& node) = 0;
};
}  // namespace angreal

#endif  // ANGREAL_VISITOR_H
