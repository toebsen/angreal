//
// Created by bichlmaier on 07.02.2020.
//

#ifndef TBLANG_VISITOR_H
#define TBLANG_VISITOR_H

#include <string>

namespace tb_lang {

namespace parser::AST {
class Program;
class Block;
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
}  // namespace parser::AST

using namespace parser::AST;

class Visitor : public std::enable_shared_from_this<Visitor>, private NonCopyable {
   public:
    virtual void visit(std::shared_ptr<Program> node) = 0;
    virtual void visit(std::shared_ptr<Block> node) = 0;
    virtual void visit(std::shared_ptr<Declaration> node) = 0;
    virtual void visit(std::shared_ptr<Assignment> node) = 0;
    virtual void visit(std::shared_ptr<Return> node) = 0;
    virtual void visit(std::shared_ptr<IdentifierLiteral> node) = 0;
    virtual void visit(std::shared_ptr<BoolLiteral> node) = 0;
    virtual void visit(std::shared_ptr<IntLiteral> node) = 0;
    virtual void visit(std::shared_ptr<FloatLiteral> node) = 0;
    virtual void visit(std::shared_ptr<StringLiteral> node) = 0;
    virtual void visit(std::shared_ptr<UnaryOperation> node) = 0;
    virtual void visit(std::shared_ptr<BinaryOperation> node) = 0;
    virtual void visit(std::shared_ptr<FunctionCall> node) = 0;
    virtual void visit(std::shared_ptr<FormalParameter> node) = 0;
    virtual void visit(std::shared_ptr<FunctionDeclaration> node) = 0;
};
}  // namespace tb_lang

#endif  // TBLANG_VISITOR_H
